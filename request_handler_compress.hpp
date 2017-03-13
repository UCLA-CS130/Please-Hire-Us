#ifndef COMPRESSION_HANDLER
#define COMPRESSION_HANDLER

#include "config_parser.h"
#include "request_handler.hpp"
#include <memory>
#include <unordered_map>
#include <unordered_set>

std::unordered_set<std::string> supportedEncodings = {"gzip", "deflate"};

class CompressionHandler : public RequestHandler {
public:
  CompressionHandler();
  virtual RequestHandler::Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual RequestHandler::Status HandleRequest(const Request& request, Response* response);
  std::unordered_map<std::string, double> extractEncodings(const std::string& input);
};

#endif
