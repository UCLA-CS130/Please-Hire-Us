#include "request_handler_echo.hpp"
#include "response.hpp"
#include <iostream>
#include <memory>


NotFoundHandler::NotFoundHandler(){}

RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix, const NginxConfig& config){
  _uri_prefix = uri_prefix;
  _config = config;

  return RequestHandler::OK;

}

RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request, Response* response){

  response->SetStatus(Response::NOT_FOUND);
  std::string content_header_name = "Content-Type";
  std::string content_header = "text/plain";

  response->AddHeader(content_header_name, content_header);
  response->SetBody("404 - NOT FOUND");
  
  return RequestHandler::OK;
}
