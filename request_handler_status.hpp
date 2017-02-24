#ifndef STATUS_HANDLER
#define STATUS_HANDLER

#include "config_parser.h"
#include "request_handler.hpp"
#include <memory>


class StatusHandler : public RequestHandler {
public:
  StatusHandler();
  virtual RequestHandler::Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual Response::ResponseCode HandleRequest(const Request& request, Response* response);
};

#endif
