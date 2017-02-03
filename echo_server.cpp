#include <boost/asio.hpp>
#include "echo_server.hpp"
#include "config_parser.h"
#include "httpRequest.hpp"


EchoServer::EchoServer(NginxConfig inputConfig) :
  io_service_(),
  acceptor_(io_service_),
  config_(inputConfig)
{}


int EchoServer::getPort(){
  return port;
}

bool EchoServer::extractConfig(std::string& errorMessage){
  
  std::string str_port = "";
  for (auto statements : config_.statements_){
    if (statements->tokens_[0] == "server" && statements->child_block_ != nullptr){
      for (auto childStatement : statements->child_block_->statements_){
        if (childStatement->tokens_[0] == "listen" || childStatement->tokens_[0] == "port"){
          str_port = childStatement->tokens_[1];
          port = stoi(str_port);
	  break;
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
bool EchoServer::init(std::string& errorMessage){

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


void EchoServer::run(){
  std::cout << "Running echo_server..." << std::endl << std::endl;

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

    std::string response = httpReq.handle_request();

    std::size_t bytes_written = socket.write_some(boost::asio::buffer(response), error);
    if (bytes_written == 0){
      std::cerr << "Http response could not be written; ERROR: " << error << std::endl;
      return;
    }
  }
}
