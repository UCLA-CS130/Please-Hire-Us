#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER

#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include <memory>


//Abstract base class
class RequestHandler {
public:
  RequestHandler();
  virtual bool handle_request(const HttpRequest& request, HttpResponse* &response) = 0;
};

#endif
