#include <string>
#include <iostream>
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include <boost/regex.hpp>


HttpRequest::HttpRequest(std::string inputRequest) : _request(inputRequest){}

std::string HttpRequest::getPath() const {
  return _path;
}

std::string HttpRequest::getMethod() const {
  return _method;
}

std::string HttpRequest::getRequest() const {
  return _request;
}

bool HttpRequest::isEcho() const {
  return _isEcho;
}

bool HttpRequest::parse(){
  //Set request type, path, method
  boost::regex e{"^([a-zA-Z]+) (\/[\?a-zA-Z0-9.]*)"};
  boost::smatch what;
  if (boost::regex_search(_request, what, e)){
    if (what.size() == 2){
      _method = what.captures(0)[0];
      _path = what.captures(1)[0];
      return true;
    }
    else
      return false;
  }
  return false;
}

