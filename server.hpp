#include <boost/asio.hpp>
#include "config_parser.h"
#include <string>
#include "request_handler.hpp"
#include <unordered_set>
#include <map>


const int MAX_REQUEST_SIZE = 8192;
const std::unordered_set<std::string>handlers = {"EchoHandler", "StaticHandler"}; 

class Server {
 public:
   Server(NginxConfig inputConfig);
   int getPort();
   bool init(std::string& errorMessage);
   bool extractConfig(std::string& errorMessage);
   bool addHandler(const std::string& handlerName, const std::string& uri_prefix, const NginxConfig& sub_config);
   void run();

 private:
   boost::asio::io_service io_service_;
   boost::asio::ip::tcp::acceptor acceptor_;
   NginxConfig config_;
   RequestHandler* _handler;
  
   //Container to map uri_prefix to appropriate RequestHandler
   std::map<std::string, std::unique_ptr<RequestHandler>> _handlerContainer;
   int port;
};
