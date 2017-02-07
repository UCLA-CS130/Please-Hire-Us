#include <boost/asio.hpp>
#include "server.hpp"
#include "config_parser.h"
#include "httpRequest.hpp"
#include "request_handler_echo.hpp"


Server::Server(NginxConfig inputConfig) :
  io_service_(),
  acceptor_(io_service_),
  config_(inputConfig)
{}


int Server::getPort(){
  return port;
}

bool Server::extractConfig(std::string& errorMessage){
  
  static_root = "";
  std::string str_port = "";
  for (auto statements : config_.statements_){
    if (statements->tokens_[0] == "server" && statements->child_block_ != nullptr){
      for (auto childStatement : statements->child_block_->statements_){
	
	//Extract port
        if (childStatement->tokens_.size() >= 1 && (childStatement->tokens_[0] == "listen" || childStatement->tokens_[0] == "port")){
          str_port = childStatement->tokens_[1];
          port = stoi(str_port);
        }
	//Extract root dir for path /static
        else if (childStatement->tokens_[0] == "path" && childStatement->tokens_[1] == "/static"){
	  for (auto staticConfig : childStatement->child_block_->statements_){
	    if (staticConfig->tokens_.size() >= 2 && staticConfig->tokens_[0] == "root"){
	      static_root = staticConfig->tokens_[1];
	    }
	  }
        }
      } 
    }
  }

  if (str_port == ""){
    errorMessage = "No port provided.";
    return false; 
  }

  if (static_root == ""){
    errorMessage = "No static root dir provided in config.";
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
  std::cout << "Running echo_server with static root " << static_root << "..." << std::endl << std::endl;

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
    //httpReq.parse();
    
    /*TODO: 
      1. Parse request
      2. Init request handler based on specified path
      3. Call handle_request()
    */
  
    EchoHandler echo_handler;
    bool handle_status = echo_handler.handle_request(httpReq, response);
    if (!handle_status || response == NULL)
      break;
   
    std::string response_str = response->toString();
    std::size_t bytes_written = socket.write_some(boost::asio::buffer(response_str), error);
    if (bytes_written == 0){
      std::cerr << "Http response could not be written; ERROR: " << error << std::endl;
      return;
    }
    delete(response);
  }
}
