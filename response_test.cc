#include "gtest/gtest.h"
#include "response.hpp"
#include <iostream>

TEST(ResponseToStringTest, validResponse){
  Response response;
  response.SetStatus(Response::OK);
  std::string content_header_name = "Content-Type";
  std::string content_header = "text/plain";

  response.AddHeader(content_header_name, content_header);
  response.SetBody("This is the body");

  std::string response_str = "HTTP/1.1 200\r\nContent-Type: text/plain\r\n\r\nThis is the body";
  EXPECT_STREQ(response_str.c_str(), response.ToString().c_str());
}

TEST(ResponseGetStatusTest, validGetStatus){
  Response response;
  response.SetStatus(Response::OK);
  
  Response::ResponseCode response_OK = Response::OK;
  EXPECT_EQ(response_OK, response.getStatus());
}
