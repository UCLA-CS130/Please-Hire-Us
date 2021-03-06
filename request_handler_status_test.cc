#include "gtest/gtest.h"
#include "request_handler.hpp"
#include "request_handler_status.hpp"
#include "request.hpp"
#include "response.hpp"
#include <iostream>
#include "config_parser.h"


TEST(StatusHandlerTest, validInit) {
  StatusHandler handler;
  std::string uri_prefix = "/test";
  NginxConfig config;

  RequestHandler::Status status_handler_status = handler.Init(uri_prefix, config);
  EXPECT_EQ(status_handler_status, 200);
}

TEST(StatusHandlerTest, validHandleRequest) {
  StatusHandler handler;
  Response response;

  std::string raw_request = 
  "GET /status HTTP/1.1\r\n"
  "Host: localhost:8008"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive\r\n\r\n";

  auto request = Request::Parse(raw_request);
  std::string empty_config = "";
  std::stringstream config_stream(empty_config);

  NginxConfigParser parser;
  NginxConfig out_config;
  //parser.Parse(&config_stream, &out_config);

  handler.Init("/status", out_config);
  //RequestHandler::Status status_handler_status = handler.HandleRequest(*request, &response);
  //EXPECT_EQ(status_handler_status, 200);
  //EXPECT_EQ(response.getStatus(), 200);
}
