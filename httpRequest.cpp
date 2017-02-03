#include <string>
#include <iostream>
#include "httpRequest.hpp"
#include "httpResponse.hpp"

//TODO: Create a parse function that extracts http request params

HttpRequest::HttpRequest(std::string inputRequest) : request(inputRequest){}

std::string HttpRequest::handle_request(){
  
  std::string response_code = "200";
  std::string content_type = "text/plain";

  HttpResponse httpRes(response_code, content_type, request);
  if (!httpRes.checkValidity()){
    std::cerr << "Http response is invalid." << std::endl;
    return "";
  }

  return httpRes.toString();
}
