#ifndef ECHO_HANDLER
#define ECHO_HANDLER

#include "request_handler.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include <memory>


class EchoHandler : public RequestHandler {
public:
  EchoHandler();
  virtual bool handle_request(const HttpRequest& request, HttpResponse* &response);
};

#endif
