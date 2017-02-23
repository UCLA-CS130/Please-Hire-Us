#ifndef NOTFOUND_HANDLER
#define NOTFOUND_HANDLER

#include "config_parser.h"
#include "request_handler.hpp"
#include <memory>


class NotFoundHandler : public RequestHandler {
public:
  NotFoundHandler();
  virtual RequestHandler::Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual Response::ResponseCode HandleRequest(const Request& request, Response* response);
};

#endif
