#include "gtest/gtest.h"
#include "request_handler.hpp"
#include "request_handler_static.hpp"
#include "config_parser.h"
#include "request.hpp"
#include "response.hpp"
#include <iostream>


TEST(StaticHandlerTest, validRequest){
  StaticHandler handler;
  NginxConfig out_config;


  std::string raw_request = 
  "GET /static/dogs.gif HTTP/1.1\r\n"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive\r\n\r\n";

  std::string config = "root /home/mpgermano/CS130/Please-Hire-Us/www/;\n";
                       
  NginxConfigParser parser;
  std::stringstream config_stream(config);

  bool success = parser.Parse(&config_stream, &out_config);
  ASSERT_TRUE(success);

  std::unique_ptr<Request> req = Request::Parse(raw_request);
  Response * response = new Response();

  handler.Init("/static", out_config);
  RequestHandler::Status response_status = handler.HandleRequest(*req, response);
  
  EXPECT_EQ(response_status, RequestHandler::OK);
  delete(response);
}

TEST(StaticHandlerTest, validMIMEType){
  std::string valid_filepath = "/dogs.gif";
  std::string root_dir = "/home/gil/cs130/Please-Hire-Us/www/";
  std::string content_type;

  StaticHandler handler;
  bool success = handler.getMIMEType(valid_filepath, &content_type);
  
  ASSERT_TRUE(success);
  EXPECT_EQ(content_type, "image/gif");
}
