#include "gtest/gtest.h"
#include "request_handler.hpp"
#include "request_handler_echo.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include <iostream>


TEST(EchoHandlerTest, validRequest){
  std::string valid_request = 
  "GET / HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive";

  HttpRequest httpReq(valid_request);
  HttpResponse* response;
  EchoHandler handler;
  handler.handle_request(httpReq, response);
  std::string response_str = response->toString();
  std::string full_res = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n" + valid_request + "\n\n";
  
  EXPECT_STREQ(full_res.c_str(), response_str.c_str());
  delete(response);
}


TEST(EchoHandlerTest, validRequest2){
  std::string valid_request2 = 
  "GET /sample.txt HTTP/1.1"
  "Host: 10.97.10.103:3000"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive";

  HttpRequest httpReq(valid_request2);
  HttpResponse* response;
  EchoHandler handler;
  handler.handle_request(httpReq, response);
  std::string response_str = response->toString();
  std::string full_res = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n" + valid_request2 + "\n\n";
  
  EXPECT_STREQ(full_res.c_str(), response_str.c_str());
  delete(response);
}
