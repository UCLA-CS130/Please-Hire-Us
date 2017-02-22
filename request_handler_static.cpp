#include "request_handler.hpp"
#include "request_handler_static.hpp"
#include "config_parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <memory>


const std::unordered_map<std::string, std::string> MIMEmap  = {
 { "html", "text/html"},
 { "css", "text/css"},
 { "js", "text/javascript"},
 { "png", "image/png"},
 { "gif", "image/gif"},
 { "jpeg", "image/jpeg"},
 { "jpg", "image/jpeg"},
 { "txt", "text/plain"},
 { "", "text/plain"}
};


RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix, const NginxConfig& config){
  _uri_prefix = uri_prefix;
  _config = config;
  _root = "";

  for (auto statement : config.statements_) {
    if (statement->tokens_[0] == "root" && statement->tokens_.size() > 1)
      _root = statement->tokens_[1];
  }   

  if (_root == "")
    return RequestHandler::ERROR;

  return RequestHandler::OK;
}

RequestHandler::Status StaticHandler::HandleRequest(const Request& request, Response* response){
  std::stringstream data;
  
  std::string uri = request.uri();
 
  std::size_t slash_pos = uri.find("/", 1);
  std::string file_path = uri.substr(slash_pos + 1);

  //Ensure only 1 slash in full path when concatenated
  if (_root[_root.size() - 1] == '/' && file_path[0] == '/'){
    _root = _root.substr(0, _root.size() - 1);
  }

  std::string full_path = _root + file_path;
  std::string content_type = "";
  bool recognizedFileType = getMIMEType(file_path, &content_type);
  
  if (!recognizedFileType){
    response->SetStatus(Response::UNSUPPORTED_MEDIA_TYPE);
    response->SetBody("415: Unsupported file extension given");
    response->AddHeader("Content-Type", content_type);
    return OK;
  }

  std::ifstream fil;
  fil.open(full_path, std::ifstream::in);
  if (!fil.is_open()){
    response->SetStatus(Response::NOT_FOUND);
    response->SetBody("404: File not found");
    response->AddHeader("Content-Type", "text/plain");
    return OK;
  }

  char nextChar = fil.get();
  std::string fileData;
  while (fil.good()){
    fileData.push_back(nextChar);
    nextChar = fil.get();
  }

  response->SetStatus(Response::OK);
  response->SetBody(fileData);
  response->AddHeader("Content-Type", content_type);
  fil.close();
  return OK;
}

bool StaticHandler::getMIMEType(const std::string& file_name, std::string * content_type){
  unsigned int extLen = 0;
  //First get the extension
  for (int i = file_name.size() - 1; i >= 0; i--){
    if (file_name[i] == '.')
      break;

    extLen++;
  }
  
  if (extLen == file_name.size())
    return false;
  
  std::string extension = file_name.substr(file_name.size() - extLen, extLen);
  
  auto it = MIMEmap.find(extension);
  if (it == MIMEmap.end())
    return false;

  *content_type = it->second;
  return true;
}


