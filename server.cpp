#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include "server.hpp"
#include "config_parser.h"
#include "httpRequest.hpp"
#include "request_handler_echo.hpp"
#include "request_handler_static.hpp"
#include <unordered_map>


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
  for (auto statements : config_.statements_){
    if (statements->tokens_[0] == "server" && statements->child_block_ != nullptr){
      for (auto childStatement : statements->child_block_->statements_){
	
	//Extract port
        if (childStatement->tokens_.size() >= 1 && (childStatement->tokens_[0] == "listen" || childStatement->tokens_[0] == "port")){
          str_port = childStatement->tokens_[1];
          port = stoi(str_port);
        }

	/* Since the config format is:
          path /static StaticHandler
	  first check that there are three tokens
          then ensure that the handler specified is valid
          then store the path mapping to <handler,root> pair*/
         
        else if (childStatement->tokens_.size() == 3 && childStatement->tokens_[0] == "path" && handlers.find(childStatement->tokens_[2]) != handlers.end()){
          std::string handler = childStatement->tokens_[2];
          std::string path = childStatement->tokens_[1];
          std::string root = "";
	  for (auto handlerConfig : childStatement->child_block_->statements_){
	    if (handlerConfig->tokens_.size() >= 2 && handlerConfig->tokens_[0] == "root"){
	      root = handlerConfig->tokens_[1];
            }
	  }
          std::pair<std::string, std::string> p(handler, root);
	  path_to_handler.insert(std::make_pair(path, p));
        }
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

// Take config file and extract necessary details and init acceptor to listen
bool Server::init(std::string& errorMessage){

  bool validConfig = extractConfig(errorMessage);
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


void Server::run(){
  std::cout << "Running echo_server... " << std::endl << std::endl;
  
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
    std::string request(req_buf);
    std::string errorMessage;
    
    HttpRequest httpReq(request);
    HttpResponse* response;
    if (!httpReq.parse()){
      return; 
    }

    std::string requested_path = httpReq.getPath();
    auto it = path_to_handler.find(requested_path);
    if (it == path_to_handler.end())
      _handler = new RequestHandler();
     
    else if (it->second.first == "EchoHandler")
      _handler = new EchoHandler();

    else if (it->second.first == "StaticHandler")
      _handler = new StaticHandler(it->second.second);

    else
      _handler = new RequestHandler();

    bool handle_status;
    handle_status = _handler->handle_request(httpReq, response);

    if (!handle_status || response == NULL){
      std::cout << "now down here" << std::endl;
      if (!response)
        std::cout << "response is null " << std::endl;
      break;
   }
    std::string response_str = response->toString();
    std::size_t bytes_written = socket.write_some(boost::asio::buffer(response_str), error);

    if (bytes_written == 0){
      std::cerr << "Http response could not be written; ERROR: " << error << std::endl;
      return;
    }

    delete(response);
    if (_handler)
      delete(_handler);
  }
}
