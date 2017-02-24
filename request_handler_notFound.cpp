#include "request_handler_notFound.hpp"
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

  response->SetStatus(Response::OK);
  std::string content_header_name = "Content-Type";
  std::string content_header = "text/plain";

  response->AddHeader(content_header_name, content_header);
  response->SetBody("404 error - File Not Found");
  
  return RequestHandler::OK;
}
