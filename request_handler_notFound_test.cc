#include "gtest/gtest.h"
#include "request_handler.hpp"
#include "request_handler_notFound.hpp"
#include "request.hpp"
#include "response.hpp"
#include <iostream>

TEST(NotFoundHandlerTest, validInit) {
  NotFoundHandler handler;
  std::string uri_prefix = "/test";
  NginxConfig config;

  RequestHandler::Status not_found_status = handler.Init(uri_prefix, config);
  EXPECT_EQ(not_found_status, 200);
}

TEST(NotFoundHandlerTest, validHandleRequest) {
  NotFoundHandler handler;
  Response response;

  std::string raw_request = 
  "GET /notfound HTTP/1.1\r\n"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive\r\n\r\n";

  auto request = Request::Parse(raw_request);

  RequestHandler::Status not_found_status = handler.HandleRequest(*request, &response);
  EXPECT_EQ(not_found_status, 404);
  EXPECT_EQ(response.getStatus(), 404);
}
