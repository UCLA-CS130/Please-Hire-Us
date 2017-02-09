#include "request_handler.hpp"
#include "request_handler_static.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

const std::map<std::string, std::string> MIMEmap  = {
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


StaticHandler::StaticHandler(std::string root_dir) : _root(root_dir){
}

bool StaticHandler::handle_request(const HttpRequest& request, HttpResponse* &response){
  std::stringstream data;

  std::string response_code;
  std::string body;
  //first ensure that root_dir doesn't end with slash AND filePath doesn't begin with slash
  std::string file_path = request.getFilePath();

  if (_root[_root.size() - 1] == '/' && file_path[0] == '/'){
    _root = _root.substr(0, _root.size() - 1);
  }

  std::string full_path = _root + file_path;
  bool recognizedFileType = getMIMEType(file_path);
  if (!recognizedFileType){
    response_code = "415";
    _content_type = "text/plain";
    body = "415: Unsupported file extension given";
    response = new HttpResponse(response_code, _content_type, body);
    return true;
  }

  std::ifstream fil(full_path);
  if (!fil.good()){
    response_code = "404";
    body = "404: File not found";
    response = new HttpResponse(response_code, _content_type, body);
    return true;
  }
  char nextChar = fil.get();
  std::string fileData;
  while (fil.good()){
    fileData.push_back(nextChar);
    nextChar = fil.get();
  }

  fil.close();
  response_code = "200";
  response = new HttpResponse(response_code, _content_type, fileData);
  return true;
}

bool StaticHandler::getMIMEType(std::string file_name){
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

  _content_type = it->second;
  return true;
}


