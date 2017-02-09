#include "request_handler.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"


RequestHandler::RequestHandler(){}
RequestHandler::~RequestHandler(){}

bool RequestHandler::handle_request(const HttpRequest& request, HttpResponse* &response){
  std::string body = "No handler for provided path. Please check the config file provided for valid path(s).";
  std::string response_code = "404";
  std::string content_type = "text/plain";
  response = new HttpResponse(response_code, content_type, body);
  return true;
}
