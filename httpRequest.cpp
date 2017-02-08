#include <string>
#include <iostream>
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include <boost/regex.hpp>


HttpRequest::HttpRequest(std::string inputRequest) : _request(inputRequest){}

std::string HttpRequest::getPath() const {
  return _path;
}

std::string HttpRequest::getFilePath() const {
  return _filePath;
}

std::string HttpRequest::getMethod() const {
  return _method;
}

std::string HttpRequest::getRequest() const {
  return _request;
}

bool HttpRequest::parse(){
  //Set request type, path, method
  boost::regex full_expr{"^([a-zA-Z]+) (\\/[\?a-zA-Z0-9.i\\/]*)"};
  boost::regex path_expr{"(\\/[\?a-zA-Z0-9]*)"};
  boost::smatch what;
  
  if (boost::regex_search(_request, what, full_expr)){
       if (what.size() < 3)
         return false; 

      _method = what[1];
      std::string full_path = what[2];
      
      boost::regex_search(full_path, what, path_expr);
      _path = what[0];
      _filePath = full_path.substr(_path.size(), full_path.size() - _path.size());

      return true;
  }
  return false;
}

