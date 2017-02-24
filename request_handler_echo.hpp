#ifndef ECHO_HANDLER
#define ECHO_HANDLER

#include "config_parser.h"
#include "request_handler.hpp"
#include <memory>


class EchoHandler : public RequestHandler {
public:
  EchoHandler();
  virtual RequestHandler::Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual RequestHandler::Status HandleRequest(const Request& request, Response* response);
};

#endif
