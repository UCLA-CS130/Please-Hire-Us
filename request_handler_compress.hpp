#ifndef COMPRESSION_HANDLER
#define COMPRESSION_HANDLER

#include "config_parser.h"
#include "request_handler.hpp"
#include <memory>


class CompressionHandler : public RequestHandler {
public:
  CompressionHandler();
  virtual RequestHandler::Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual RequestHandler::Status HandleRequest(const Request& request, Response* response);
};

#endif
