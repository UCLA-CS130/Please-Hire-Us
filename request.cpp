#include <string>
#include <iostream>
#include "request.hpp"
#include <boost/regex.hpp>
#include <memory>


std::unique_ptr<Request> Request::Parse(const std::string& raw_request){

  std::unique_ptr<Request> req = std::unique_ptr<Request>(new Request);
  req->_raw_request = raw_request;
  
  //Set request type, path, method
  boost::regex first_line_expr{"^([a-zA-Z]+) (\\/[\?a-zA-Z0-9.\\/]*) ([a-zA-Z0-9.\\/]*)"};
  //boost::regex path_expr{"(\\/[\?a-zA-Z0-9]*)"};
  boost::smatch what;
  
  if (boost::regex_search(raw_request, what, first_line_expr)){
    if (what.size() < 4)
      return false; 

    req->_method = what[1];
    req->_uri = what[2];
    req->_version = what[3];

  }

  boost::regex header_expr{"([a-zA-Z-]*): ([^\n]*)"};
  std::string::const_iterator searchStart (raw_request.cbegin());

  while (boost::regex_search(searchStart, raw_request.cend(), what, header_expr)){
    std::string header_name;
    std::string header_value;

    if (what.length() < 3)
      continue;
 
    header_name = what[1];
    header_value = what[2]; 
    
    searchStart += what.position() + what.length();
    std::pair<std::string, std::string> header_pair(header_name, header_value);
    req->_headers.push_back(header_pair);
  }
  
  req->_body = "";
  return req;
}

std::string Request::raw_request() const{
  return _raw_request;
}

std::string Request::method() const{
  return _method;
}

std::string Request::uri() const{
  return _uri;
}

std::string Request::version() const{
  return _version;
}

std::string Request::body() const{
  return _body;
}

Request::Headers Request::headers() const {
  return _headers;
}
