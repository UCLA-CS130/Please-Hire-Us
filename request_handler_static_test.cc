#include "gtest/gtest.h"
#include "request_handler.hpp"
#include "request_handler_static.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include <iostream>


TEST(StaticHandlerTest, validRequest){
  std::string valid_request = 
  "GET /static/dogs.gif HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive";
  std::string root_dir = "/home/gil/cs130/Please-Hire-Us/";

  HttpRequest httpReq(valid_request);
  HttpResponse* response;
  StaticHandler handler(root_dir);
  bool success = handler.handle_request(httpReq, response);
  
  EXPECT_TRUE(success);
  delete(response);
}

TEST(StaticHandlerTest, validMIMEType){
  std::string valid_filepath = "/dogs.gif";
  std::string root_dir = "/home/gil/cs130/Please-Hire-Us/";


  StaticHandler handler(root_dir);
  bool success = handler.getMIMEType(valid_filepath);
  
  EXPECT_TRUE(success);
}
