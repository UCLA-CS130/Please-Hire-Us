#include <boost/asio.hpp>
#include <boost/regex.hpp>

#include "server.hpp"
#include "config_parser.h"
#include "request.hpp"
#include "response.hpp"
#include "request_handler_echo.hpp"
#include "request_handler_static.hpp"
#include "request_handler_notFound.hpp"
#include "request_handler_status.hpp"
#include "request_handler_reverse_proxy.hpp"

#include <unordered_map>
#include <iterator>
#include <vector>


Server::Server(NginxConfig inputConfig) :
  io_service_(),
  acceptor_(io_service_),
  config_(inputConfig)
{}


int Server::getPort(){
  return port;
}

bool Server::extractConfig(std::string& errorMessage){
  
  std::string str_port = "";
  for (std::vector<std::shared_ptr<NginxConfigStatement>>::iterator it = config_.statements_.begin(); it != config_.statements_.end(); it++){ 

    //Extract Port 
    if ((*it)->tokens_[0] == "port" && (*it)->tokens_.size() > 1){
      str_port = (*it)->tokens_[1];
      port = stoi(str_port);
    }
    
    //Extract default handler for errors
    else if ((*it)->tokens_[0] == "default" && (*it)->tokens_.size() > 1){
      std::string handler = (*it)->tokens_[1];
      std::string uri_prefix = "NotFound";
      
      NginxConfig handlerConfig; 
       if ((*it)->child_block_)
         handlerConfig = (*(*it)->child_block_); 
       else {
         errorMessage = "Handler " + handler + " needs configuration blocks {}.";
         return false;
       }
      
      //Add Handler to private member _handlerContainer
       if (!addHandler(handler, uri_prefix, handlerConfig)){
         errorMessage = "Could not add handler: " + handler + " with uri_prefix: " + uri_prefix;
         return false;
       }
    }
 
    //Extract Handler and Init them with uri_prefix and NginxConfig (subconfig for that handler)   
    else if ((*it)->tokens_.size() == 3 && (*it)->tokens_[0] == "path" && handlers.find((*it)->tokens_[2]) != handlers.end()){
       std::string handler = (*it)->tokens_[2];
       std::string uri_prefix = (*it)->tokens_[1];
      
       NginxConfig handlerConfig; 
       if ((*it)->child_block_)
         handlerConfig = (*(*it)->child_block_); 
       else {
         errorMessage = "Handler " + handler + " needs configuration blocks {}.";
         return false;
       }

       //Add Handler to private member _handlerContainer
       if (!addHandler(handler, uri_prefix, handlerConfig)){
         errorMessage = "Could not add handler: " + handler + " with uri_prefix: " + uri_prefix;
         return false;
       }
     }
   } 

  if (str_port == ""){
    errorMessage = "No port provided.";
    return false; 
  }

  if (port < 0 || port > 65535){
    errorMessage = "Port given was outside of acceptable range (0 - 65535).";
    return false;
  }

  return true;
}

bool Server::addHandler(const std::string& handlerName, const std::string& uri_prefix, const NginxConfig& sub_config){
    
    if (handlerName == "EchoHandler"){
      _handlerContainer.emplace(uri_prefix, std::unique_ptr<RequestHandler>(new EchoHandler()));
      pathToHandler.emplace(uri_prefix, "EchoHandler");
    }
    else if (handlerName ==  "StaticHandler"){
      _handlerContainer.emplace(uri_prefix, std::unique_ptr<RequestHandler>(new StaticHandler()));
      pathToHandler.emplace(uri_prefix, "StaticHandler");
    }
    else if (handlerName == "StatusHandler"){
      _handlerContainer.emplace(uri_prefix, std::unique_ptr<RequestHandler>(new StatusHandler()));
      pathToHandler.emplace(uri_prefix, "StatusHandler");
    }
    else if (handlerName == "ReverseProxyHandler") {
      _handlerContainer.emplace(uri_prefix, std::unique_ptr<RequestHandler>(new ReverseProxyHandler()));
      pathToHandler.emplace(uri_prefix, "ReverseProxyHandler");
    }
    else{
      _handlerContainer.emplace(uri_prefix, std::unique_ptr<RequestHandler>(new NotFoundHandler()));
      pathToHandler.emplace(uri_prefix, "NotFoundHandler");
    }

    RequestHandler::Status status = _handlerContainer[uri_prefix]->Init(uri_prefix, sub_config);
    
    //So request object can access the server public info
    _handlerContainer[uri_prefix]->setServer(this);
    if (status == RequestHandler::OK)
      return true;
    else
      return false;
}

// Take config file and extract necessary details and init acceptor to listen
bool Server::init(std::string& errorMessage){

  bool validConfig = extractConfig(errorMessage);
  std::cout << errorMessage << std::endl;
  if (!validConfig){
    return false;
  }

  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  std::cout << "Server now listening on port: " << port << std::endl;
  return true;

}

//Create a string for the status handler
std::string Server::getStatus(){
  std::string ret = "Server has received " + std::to_string(requestArchive.size()) + " requests.\n-------------\n";
  for (auto it = requestArchive.begin(); it != requestArchive.end(); it++)
    ret += "Request Path: " + it->first + ", Response: " + std::to_string(it->second) + "\n";
  
  
  ret += "\nPath to handler mapping: \n";
  
  for (auto it = pathToHandler.begin(); it != pathToHandler.end(); it++)
    ret += "Path: " + it->first + ", Handler: " + it->second + "\n";
  
  return ret;
}


void Server::run(){
  std::cout << "Running server... " << std::endl << std::endl;

  for(;;){
    boost::asio::ip::tcp::socket socket(io_service_);
    acceptor_.accept(socket);

    // This will be set on any error from socket.read_some
    boost::system::error_code error;

    // MAX request size is 8 KB
    char req_buf[MAX_REQUEST_SIZE];
    memset(req_buf, 0, MAX_REQUEST_SIZE);

    std::size_t bytes_read = socket.read_some(boost::asio::buffer(req_buf), error);
    
    if (bytes_read == 0){
      std::cout << "--------ERROR-------Boost Error Code-----" << error << std::endl;
      return;
    }
    std::string raw_request(req_buf);
    std::string errorMessage;
    
    auto httpRequest = Request::Parse(raw_request);
    Response  * httpResponse  = new Response();

    std::string uri = httpRequest->uri();

    bool notFound = true;
    RequestHandler::Status response_status;

    std::vector<int> slashPositions;
    for (int i = uri.size(); i >= 0; i--){
      if (uri[i] == '/')
        slashPositions.push_back(i);
    }

    if (_handlerContainer.find(uri) != _handlerContainer.end()){
      response_status = _handlerContainer[uri]->HandleRequest((*httpRequest), httpResponse);
      notFound = false;
    }

    else {
      // Loop through slashes and see if any prefixes match config paths
      for (auto it = slashPositions.begin(); it != slashPositions.end(); it++){
        std::size_t prefix_slash = uri.find("/", *it);
        if (prefix_slash == 0)
          prefix_slash++;
     
        std::string uri_prefix = uri.substr(0, prefix_slash); 
        //We check if uri_prefix is valid
        if (_handlerContainer.find(uri_prefix) != _handlerContainer.end()){
          notFound = false;
          response_status = _handlerContainer[uri_prefix]->HandleRequest((*httpRequest), httpResponse);
          break;
        }
      }
    }
   
    //No Handler for prefix
    if (notFound) 
        response_status = _handlerContainer["NotFound"]->HandleRequest((*httpRequest), httpResponse);
 
    requestArchive.emplace(uri, response_status);

    std::size_t bytes_written;
    if (response_status != RequestHandler::SERVER_ERROR){ 
      std::string response_str = httpResponse->ToString();
      bytes_written = socket.write_some(boost::asio::buffer(response_str), error);
    }
    if (bytes_written == 0){
      std::cerr << "Http response could not be written; ERROR: " << error << std::endl;
      return;
    }

    delete(httpResponse);
  }
}
