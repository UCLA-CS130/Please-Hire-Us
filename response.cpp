#include "response.hpp"
#include <vector>
#include <string>

void Response::SetStatus(const ResponseCode response_code){
  _response_code = response_code;
}

void Response::AddHeader(const std::string& header_name, const std::string& header_value, bool append){
  for (auto it = _headers.begin(); it != _headers.end(); it++){
    if (it->first == header_name && append){
      it->second += header_value;
      return;
    }
    else if (it->first == header_name && !append){
      it->second = header_value;
      return;
    }
  }

  std::pair<std::string, std::string> newHeader(header_name, header_value);
  _headers.push_back(newHeader);  
}

void Response::SetBody(const std::string& body){
  _body = body;
}

std::string Response::GetBody(){
  return _body;
}

std::string Response::ToString(){
  const std::string CRLF = "\r\n";
  std::string msg = "HTTP/1.1 " + std::to_string(_response_code) + CRLF;
  for (auto header : _headers){
    msg += header.first + ": " + header.second + CRLF;
  }
  msg += CRLF;
  msg += _body;
  return msg;
}

Response::ResponseCode Response::getStatus(){
  return _response_code;
}
