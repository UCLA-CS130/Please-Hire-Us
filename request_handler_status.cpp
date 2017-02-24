#include "request_handler_status.hpp"
#include "response.hpp"
#include <iostream>
#include <memory>


StatusHandler::StatusHandler(){}

RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix, const NginxConfig& config){
  _uri_prefix = uri_prefix;
  _config = config;

  return RequestHandler::OK;

}

RequestHandler::Status StatusHandler::HandleRequest(const Request& request, Response* response){

  response->SetStatus(Response::OK);
  std::string content_header_name = "Content-Type";
  std::string content_header = "text/plain";

  response->AddHeader(content_header_name, content_header);
  response->SetBody(parent->getStatus());
  
  return RequestHandler::OK;
}
