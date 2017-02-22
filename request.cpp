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

      /*
      boost::regex_search(full_path, what, path_expr);
      uri = what[0];
      _filePath = full_path.substr(uri.size(), full_path.size() - _path.size());
      */
  }

  //TODO: Extract header and add them to the _headers private member variable
  //TODO: Create Reg EXPR to extract body
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
