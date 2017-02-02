#include <boost/asio.hpp>
#include "echo_server.hpp"
#include "httpResponse.hpp"


EchoServer::EchoServer(NginxConfig inputConfig) :
  io_service_(),
  acceptor_(io_service_),
  config_(inputConfig)
{
 std::string str_port = config_.statements_[0]->child_block_->statements_[0]->tokens_[1];
  port = std::stoi(str_port);
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
  acceptor_.open(endpoint.protocol());
  acceptor_.bind(endpoint);
  acceptor_.listen();

  std::cout << "Server now listening on port: " << port << std::endl;
}

void EchoServer::run(){
  std::cout << "Running echo_server..." << std::endl << std::endl;

  for(;;){
    boost::asio::ip::tcp::socket socket(io_service_);
    acceptor_.accept(socket);

    // This will be set on any error from socket.read_some
    boost::system::error_code error;

    // MAX request size is 8 KB
    char req_buf[8192];
    memset(req_buf, 0, 8192);

    std::size_t bytes_read = socket.read_some(boost::asio::buffer(req_buf), error);
    
    if (bytes_read == 0){
      std::cout << "--------ERROR-------Boost Error Code-----" << error << std::endl;
    }

    boost::asio::streambuf header;
    std::ostream header_stream(&header);
    std::string body(req_buf);
    HttpResponse h("200","text/plain", body);
    std::cout << h.toString();
    header_stream << h.toString();  boost::asio::write(socket, header);
  }

}
