#include "request_handler_echo.hpp"
#include "response.hpp"
#include <iostream>
#include <memory>


EchoHandler::EchoHandler(){}

RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix, const NginxConfig& config){
  _uri_prefix = uri_prefix;
  _config = config;

  return RequestHandler::OK;

}

RequestHandler::Status EchoHandler::HandleRequest(const Request& request, Response* response){

  response->SetStatus(Response::OK);
  std::string content_header_name = "Content-Type";
  std::string content_header = "text/plain";

  response->AddHeader(content_header_name, content_header);
  response->SetBody(request.raw_request());
  
  return RequestHandler::OK;
}
