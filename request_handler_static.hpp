#ifndef static_handler
#define static_handler

#include "request_handler.hpp"


class StaticHandler : public RequestHandler {
public:
  virtual RequestHandler::Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual Response::ResponseCode HandleRequest(const Request& request, Response* response);
  bool getMIMEType(const std::string& file_name, std::string * content_type);

private:
  std::string _root;
};

#endif

