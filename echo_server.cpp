#include <boost/asio.hpp>
#include "echo_server.hpp"


EchoServer::EchoServer(NginxConfig inputConfig) :
  io_service_(),
  acceptor_(io_service_),
  config_(inputConfig)
{
  //TODO: Don't hardcode the port extraction as done below
  std::string str_port = config_.statements_[0]->child_block_->statements_[0]->tokens_[1];
  port = std::stoi(str_port);  // May just be able to leave this as a string

  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
  acceptor_.open(endpoint.protocol());
  acceptor_.bind(endpoint);
  acceptor_.listen();

  std::cout << "Server now listening on port: " << port << std::endl;
}

void EchoServer::run(){
  std::cout << "Running echo_server..." << std::endl << std::endl;

  while(1){
    boost::asio::ip::tcp::socket socket(io_service_);
    acceptor_.accept(socket);

    // This will be set on any error from socket.read_some
    boost::system::error_code error;

    // MAX request size is 8 KB
    char req_buf[8192];

    std::size_t bytes_read = socket.read_some(boost::asio::buffer(req_buf), error);
    if (bytes_read == 0){
      std::cout << "--------ERROR-------Boost Error Code-----" << error << std::endl;
    }
    else {
      std::cout << "--------Request----------\n" << req_buf << std::endl;
    }
    /*
      TODO: 
      Set response code
      Set content type
      Copy request to response
    */

  }

}
