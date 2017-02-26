#include "gtest/gtest.h"
#include "request.hpp"
#include "response.hpp"
#include <iostream>


TEST(HttpRequestTest, ValidParseRequest){
  std::string valid_request = 
  "GET /static/dogs.gif HTTP/1.1\n"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive";

  auto httpReq = Request::Parse(valid_request);
  EXPECT_EQ("GET", httpReq->method());
  EXPECT_EQ("/static/dogs.gif", httpReq->uri());
  EXPECT_EQ("HTTP/1.1", httpReq->version());
  
}

TEST(HttpRequestTest, InvalidParseRequest){
  std::string invalid_request = 
  "/static/dogs.gif GET";

  auto badReq = Request::Parse(invalid_request);
  EXPECT_NE(badReq->uri(), "/static/dogs.gif");

}

