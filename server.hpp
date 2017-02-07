#include <boost/asio.hpp>
#include "config_parser.h"
#include <string>


const int MAX_REQUEST_SIZE = 8192;

class Server {
 public:
   Server(NginxConfig inputConfig);
   int getPort();
   bool init(std::string& errorMessage);
   bool extractConfig(std::string& errorMessage);
   std::string handleRequest(std::string body);
   void run();

 private:
   boost::asio::io_service io_service_;
   boost::asio::ip::tcp::acceptor acceptor_;
   NginxConfig config_;
   int port;
   std::string static_root;
};
