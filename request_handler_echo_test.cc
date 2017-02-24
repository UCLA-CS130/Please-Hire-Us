#include "gtest/gtest.h"
#include "request_handler.hpp"
#include "request_handler_echo.hpp"
#include "request.hpp"
#include "response.hpp"
#include <iostream>

TEST(EchoHandlerTest, validInit) {
  EchoHandler handler;
  std::string uri_prefix = "/test";
  NginxConfig config;

  RequestHandler::Status echo_status = handler.Init(uri_prefix, config);
  EXPECT_EQ(echo_status, 200);
}

TEST(EchoHandlerTest, validHandleRequest) {
  EchoHandler handler;
  Response response;

  std::string raw_request = 
  "GET /echo HTTP/1.1\r\n"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive\r\n\r\n";

  Request request = Request::Parse(raw_request);

  RequestHandler::Status echo_status = handler.HandleRequest(request, response);
  EXPECT_EQ(echo_status, 200);
  EXPECT_EQ(response.getStatus(), 200);
}
