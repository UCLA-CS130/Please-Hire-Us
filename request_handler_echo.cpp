#include "request_handler.hpp"
#include "request_handler_echo.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include <iostream>
#include <memory>


EchoHandler::EchoHandler(){}

bool EchoHandler::handle_request(const HttpRequest& request, HttpResponse* &response){
  std::string response_code = "200";
  std::string content_type = "text/plain";
  
  response = new HttpResponse(response_code, content_type, request.request);
  if (!response->checkValidity()){
    std::cerr << "Http response is invalid." << std::endl;
    response = NULL;
    return false;
  }
  
  if (response == NULL)
    std::cout << "Response is null " << std::endl;
  return true;
}
