#include "request_handler.hpp"
#include "request_handler_static.hpp"
#include "config_parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <memory>
#include "markdown-lib/markdown.h"


const std::unordered_map<std::string, std::string> MIMEmap  = {
 { "html", "text/html"},
 { "css", "text/css"},
 { "js", "text/javascript"},
 { "md", "text/markdown"},
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

  if (_root == ""){
    std::cerr << "No root for the static path was provided." <<std::endl;
    return RequestHandler::SERVER_ERROR;
  }

  return RequestHandler::OK;
}

RequestHandler::Status StaticHandler::HandleRequest(const Request& request, Response* response){
  std::stringstream data;
  
  std::string uri = request.uri();
 
  std::string file_path = uri.substr(_uri_prefix.size(), uri.size() - _uri_prefix.size());

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
    return RequestHandler::UNSUPPORTED_MEDIA_TYPE;
  }

  std::ifstream fil;
  fil.open(full_path, std::ifstream::in);
  if (!fil.is_open()){
    response->SetStatus(Response::NOT_FOUND);
    response->SetBody("404: File not found");
    response->AddHeader("Content-Type", "text/plain");
    return RequestHandler::NOT_FOUND;
  }

  char nextChar = fil.get();
  std::string fileData;
  while (fil.good()){
    fileData.push_back(nextChar);
    nextChar = fil.get();
  }

  std::string convertedData = fileData;
  if (content_type == "text/markdown"){
    convertedData = convertFileToMarkdown(fileData);
    content_type = "text/html";
  }
  response->SetStatus(Response::OK);
  response->SetBody(convertedData);
  response->AddHeader("Content-Type", content_type);
  fil.close();
  return RequestHandler::OK;
}

std::string StaticHandler::convertFileToMarkdown(std::string inputData){
  
  std::string markdown;
  std::ostringstream output_stream;
  
  markdown::Document mdDoc;
  mdDoc.read(inputData);
  mdDoc.write(output_stream);

  markdown = output_stream.str();

  return markdown;
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


