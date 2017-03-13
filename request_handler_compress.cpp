#include "request_handler_compress.hpp"
#include "response.hpp"
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "zlib.h"


CompressionHandler::CompressionHandler(){}

RequestHandler::Status CompressionHandler::Init(const std::string& uri_prefix, const NginxConfig& config){
  _uri_prefix = uri_prefix;
  _config = config;

  //supportedCompression.insert("gzip");
  supportedCompression.insert("deflate");
  return RequestHandler::OK;

}

RequestHandler::Status CompressionHandler::HandleRequest(const Request& request, Response* response){

  //Request is expected to be full of data at this point
  if (response->GetBody() == ""){
    response->SetBody("No data was passed to the compression handler");
    response->AddHeader("Content-Type", "text/plain");
    response->SetStatus(Response::BAD_REQUEST);
    return RequestHandler::OK;
  }

  std::string encoding_str = "";
  //TODO: Extract Accept-Encoding Header from request, prioritize based off of weighted values
    
  auto headers_ = request.headers();
  for (auto headerIt = headers_.begin(); headerIt != headers_.end(); headerIt++){
    if (headerIt->first == "Accept-Encoding"){
      encoding_str = headerIt->second; 
      break;
    }
  }

  std::unordered_map<std::string, double>encodings;
  extractEncodings(encoding_str, encodings);

  
  std::string final_encoding, compressed_data;
  double highest_weight = 0;
  for (auto encodingIt = supportedCompression.begin(); encodingIt != supportedCompression.end(); encodingIt++){
    
    auto jt = encodings.find(*encodingIt);
    if (jt != encodings.end() && jt->second > highest_weight){
      highest_weight = jt->second;
      final_encoding = jt->first;
    }
  }

  //No encoding match, so don't compress
  if (final_encoding == ""){
    return RequestHandler::Status::OK;
  }
  
  if (final_encoding == "gzip")
    compressed_data = gzipCompress(response->GetBody());
 
  else if (final_encoding == "deflate"){
    compressed_data = deflateCompress(response->GetBody()); 
  }
  response->SetBody(compressed_data);

  response->AddHeader("Content-Type", "; charset=utf-8", /*append*/true);
  response->AddHeader("Content-Encoding", final_encoding);

  std::string headers= response->ToString();
  std::size_t pos = headers.find("\r\n\r\n");
  std::string h_str = headers.substr(0, pos+1);
  return RequestHandler::OK;
}


std::string CompressionHandler::gzipCompress(const std::string& data){
 
  std::stringstream input_stream(data);
  std::stringstream output_stream;

  boost::iostreams::filtering_streambuf< boost::iostreams::input> in;
  in.push( boost::iostreams::gzip_compressor());
  in.push( input_stream);

  boost::iostreams::copy(in, output_stream);
  return output_stream.str();
}

std::string CompressionHandler::deflateCompress(const std::string& data){
 
  std::stringstream input_stream(data);
  std::stringstream output_stream;

  boost::iostreams::filtering_streambuf< boost::iostreams::input> in;
  in.push( boost::iostreams::zlib_compressor());
  in.push( input_stream);

  boost::iostreams::copy(in, output_stream);
  return output_stream.str();
}



void CompressionHandler::extractEncodings(const std::string& input, std::unordered_map<std::string,double>& encodings){
  std::size_t comma_pos = 0;
  std::size_t index = 0;
  std::size_t semicolon_pos = 0;
  std::string next_en, full_en;
  std::string priority_str;
  std::size_t q_pos = input.find("q");

  if (q_pos == std::string::npos){
    double priority = 1.0;
    while(input.find(",", index) != std::string::npos){
      comma_pos = input.find(",", index);
      next_en = input.substr(index, comma_pos - index);
      encodings.emplace(next_en, priority);
      index = comma_pos + 2;
      priority -= 0.1;
    }
   
    next_en = input.substr(index, input.size() - index - 1);
    std::pair<std::string, double> next(next_en, priority);
    encodings.emplace(next);
    return;
  }

  else {
    while (input.find(",", index) != std::string::npos){
      comma_pos = input.find(",",index);
      full_en = input.substr(index, comma_pos - index);
      semicolon_pos = full_en.find(";");
      
      if (semicolon_pos != std::string::npos){
        next_en = full_en.substr(0, semicolon_pos);
        priority_str = full_en.substr(semicolon_pos + 3, input.size() - (semicolon_pos + 3));
      }
      else {
        next_en = full_en;
        priority_str = "0.5";
      }
      index = comma_pos + 2;
      encodings.emplace(next_en, atof(priority_str.c_str()));
    }
    full_en = input.substr(index);
    semicolon_pos = full_en.find(";");
    if (semicolon_pos != std::string::npos){
      next_en = full_en.substr(0, semicolon_pos);
      priority_str = full_en.substr(semicolon_pos + 3);
    }
    else {
      next_en = full_en;
      priority_str = "0.5";
    }
    encodings.emplace(next_en, atof(priority_str.c_str()));
  } 

  return;
}


