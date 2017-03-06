#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <memory>
#include "config_parser.h"
#include "request.hpp"
#include "response.hpp"
#include "server.hpp"

class Server;

// Represents the parent of all request handlers. Implementations should expect to
// be long lived and created at server constrution.
class RequestHandler {
 public:
  enum Status {
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    UNSUPPORTED_MEDIA_TYPE = 415,
    SERVER_ERROR = 500,
    INVALID = 1
  };
  
  // Initializes the handler. Returns a response code indicating success or
  // failure condition.
  // uri_prefix is the value in the config file that this handler will run for.
  // config is the contents of the child block for this handler ONLY.
  virtual Status Init(const std::string& uri_prefix,
                      const NginxConfig& config) = 0;

  // Handles an HTTP request, and generates a response. Returns a response code
  // indicating success or failure condition. If ResponseCode is not OK, the
  // contents of the response object are undefined, and the server will return
  // HTTP code 500.
  virtual Status HandleRequest(const Request& request,
                               Response* response) = 0;
  
  //Set server so we can access server methods 
  void setServer(Server *p){
    parent = p;
  };
 protected:
  std::string _uri_prefix;
  NginxConfig _config;
  Server *parent;
};

#endif //REQUEST_HANDER_HPP
