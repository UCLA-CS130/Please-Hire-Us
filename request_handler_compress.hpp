#ifndef COMPRESSION_HANDLER
#define COMPRESSION_HANDLER

#include "config_parser.h"
#include "request_handler.hpp"
#include <memory>
#include <unordered_map>
#include <unordered_set>



class CompressionHandler : public RequestHandler {
public:
  CompressionHandler();
  virtual RequestHandler::Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual RequestHandler::Status HandleRequest(const Request& request, Response* response);
  void extractEncodings(const std::string& input, std::unordered_map<std::string, double>& encodings);
  std::string gzipCompress(const std::string& data);
  std::string deflateCompress(const std::string& data);
 private:
  std::unordered_set<std::string> supportedCompression;
};

#endif
