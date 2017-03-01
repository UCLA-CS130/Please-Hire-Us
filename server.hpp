#ifndef SERVER
#define SERVER

#include <boost/asio.hpp>
#include "config_parser.h"
#include <string>
#include "request_handler.hpp"
#include <unordered_set>
#include <unordered_map>
#include <map>

class RequestHandler;

const int MAX_REQUEST_SIZE = 8192;
const std::unordered_set<std::string>handlers = {"EchoHandler", "StaticHandler", "StatusHandler", "NotFoundHandler", "ReverseProxyHandler"}; 

class Server {
 public:
   Server(NginxConfig inputConfig);
   int getPort();
   bool init(std::string& errorMessage);
   bool extractConfig(std::string& errorMessage);
   bool addHandler(const std::string& handlerName, const std::string& uri_prefix, const NginxConfig& sub_config);
   void run();
   std::string getStatus(); 

 private:
   boost::asio::io_service io_service_;
   boost::asio::ip::tcp::acceptor acceptor_;
   NginxConfig config_;
   RequestHandler* _handler;
  
   //Container to map uri_prefix to appropriate RequestHandler
   std::map<std::string, std::unique_ptr<RequestHandler>> _handlerContainer;
   std::unordered_multimap<std::string, int> requestArchive; 
   std::unordered_multimap<std::string, std::string> pathToHandler;
   int port;
};

#endif