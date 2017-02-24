#ifndef NOT_FOUND_HANDLER
#define NOT_FOUND_HANDLER

#include "config_parser.h"
#include "request_handler.hpp"
#include <memory>


class NotFoundHandler : public NotFoundHandler {
public:
  NotFoundHandler();
  virtual RequestHandler::Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual RequestHandler::Status HandleRequest(const Request& request, Response* response);
};

#endif
