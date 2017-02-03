#include "httpResponse.hpp"
#include <string>
#include <iostream>
#include <unordered_set>


std::unordered_set<std::string> ValidResponseCode{"200", "400", "404"};
std::unordered_set<std::string> ValidContentType{"text/plain", "text/html", "text/css"};
//enum ValidResponseCode {200, 400, 404};

HttpResponse::HttpResponse(std::string responseCode, std::string contentType, std::string msgBody):responseCode(responseCode),contentType(contentType),msgBody(msgBody){}
  
bool HttpResponse::checkValidity(){
  std::unordered_set<std::string>::const_iterator it = ValidResponseCode.find(responseCode);

  if(it == ValidResponseCode.end())
    return false;

  it = ValidContentType.find(contentType);

  if(it == ValidContentType.end())
    return false;
      
  return true;
}

std::string HttpResponse::toString(){

  std::string msg = "HTTP/1.1 " + responseCode + " OK\nContent-Type: " + contentType + "\n\n" + msgBody + "\n\n";
  return msg;
}
