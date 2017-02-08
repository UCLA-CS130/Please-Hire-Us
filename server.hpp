#include <boost/asio.hpp>
#include "config_parser.h"
#include <string>
#include "request_handler.hpp"
#include <unordered_map>
#include <unordered_set>

const int MAX_REQUEST_SIZE = 8192;
const std::unordered_set<std::string>handlers = {"EchoHandler", "StaticHandler"}; 

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
   RequestHandler* _handler;
   std::unordered_map<std::string, std::pair<std::string, std::string>>  path_to_handler;
 
   int port;
};
