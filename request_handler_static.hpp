#ifndef static_handler
#define static_handler

#include "request_handler.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"


class StaticHandler : public RequestHandler {
  StaticHandler(HttpRequest& request, std::string root_dir); 
  virtual bool handle_request(HttpRequest& request, HttpResponse* response);

private:
  std::string m_root;

};

#endif

