#include <boost/asio.hpp>
#include "config_parser.h"

class EchoServer {
 public:
   EchoServer(NginxConfig inputConfig);
   void run();

 private:
   boost::asio::io_service io_service_;
   boost::asio::ip::tcp::acceptor acceptor_;

   NginxConfig config_;
   int port;
};
