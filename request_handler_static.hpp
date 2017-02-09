#ifndef static_handler
#define static_handler

#include "request_handler.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"


class StaticHandler : public RequestHandler {
public:
  StaticHandler(std::string root_dir); 
  virtual bool handle_request(const HttpRequest& request, HttpResponse* &response);
  bool getMIMEType(std::string file_name);

private:
  std::string _root;
  std::string _content_type;
};

#endif

