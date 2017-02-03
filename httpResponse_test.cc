#include "gtest/gtest.h"
#include "httpResponse.hpp"
#include <iostream>


TEST(CheckValidityTest, validResponse1){
  std::string responseCode = "200";
  std::string contentType = "text/plain";
  std::string msgBody = "GET / HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive";

  HttpResponse httpRes(responseCode, contentType, msgBody);
  bool success = httpRes.checkValidity();

  EXPECT_TRUE(success);
}

TEST(CheckValidityTest, validResponse2){
  std::string responseCode = "400";
  std::string contentType = "text/html";
  std::string msgBody = "GET / HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive";

  HttpResponse httpRes(responseCode, contentType, msgBody);
  bool success = httpRes.checkValidity();

  EXPECT_TRUE(success);
}

TEST(CheckValidityTest, invalidResponse1){
  std::string responseCode = "900";
  std::string contentType = "text/plain";
  std::string msgBody = "GET / HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive";

  HttpResponse httpRes(responseCode, contentType, msgBody);
  bool failure = httpRes.checkValidity();

  EXPECT_FALSE(failure);
}

TEST(CheckValidityTest, invalidResponse2){
  std::string responseCode = "200";
  std::string contentType = "text/funky";
  std::string msgBody = "GET / HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive";

  HttpResponse httpRes(responseCode, contentType, msgBody);
  bool failure = httpRes.checkValidity();

  EXPECT_FALSE(failure);
}

TEST(StringValidityTest, validString1){
  std::string responseCode = "200";
  std::string contentType = "text/plain";
  std::string msgBody = "GET / HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive";

  std::string full_res = 
  "HTTP/1.1 200 OK\n"
  "Content-Type: text/plain\n\n"
  "GET / HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive\n\n";

  HttpResponse httpRes(responseCode, contentType, msgBody);
  std::string response = httpRes.toString();

  EXPECT_STREQ(full_res.c_str(), response.c_str());
}

TEST(StringValidityTest, validString2){
  std::string responseCode = "200";
  std::string contentType = "text/html";
  std::string msgBody = "GET / HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive";

  std::string full_res = 
  "HTTP/1.1 200 OK\n"
  "Content-Type: text/html\n\n"
  "GET / HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive\n\n";

  HttpResponse httpRes(responseCode, contentType, msgBody);
  std::string response = httpRes.toString();

  EXPECT_STREQ(full_res.c_str(), response.c_str());
}

// Different content type creates invalid string
TEST(StringValidityTest, invalidString1){
  std::string responseCode = "200";
  std::string contentType = "text/html";
  std::string msgBody = "GET / HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive";

  std::string full_res = 
  "HTTP/1.1 200 OK\n"
  "Content-Type: text/plain\n\n"
  "GET / HTTP/1.1"
  "Host: localhost:8080"
  "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
  "Accept-Language: en-US,en;q=0.5"
  "Accept-Encoding: gzip, deflate"
  "Connection: keep-alive\n\n";

  HttpResponse httpRes(responseCode, contentType, msgBody);
  std::string response = httpRes.toString();

  EXPECT_STRNE(full_res.c_str(), response.c_str());
}