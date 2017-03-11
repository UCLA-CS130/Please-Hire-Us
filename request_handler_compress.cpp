#include "request_handler_compress.hpp"
#include "response.hpp"
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <iostream>
#include <memory>


CompressionHandler::CompressionHandler(){}

RequestHandler::Status CompressionHandler::Init(const std::string& uri_prefix, const NginxConfig& config){
  _uri_prefix = uri_prefix;
  _config = config;

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

  //TODO: Extract Accept-Encoding Header from request, prioritize based off of weighted values

  std::stringstream input_stream(response->GetBody());
  std::stringstream output_stream;

  boost::iostreams::filtering_streambuf< boost::iostreams::input> in;
  in.push( boost::iostreams::gzip_compressor());
  in.push( input_stream);

  boost::iostreams::copy(in, output_stream);
  response->SetBody(output_stream.str());

  response->AddHeader("Content-Type", "; charset=utf-8", /*append*/true);
  response->AddHeader("Content-Encoding", "gzip");

  std::string headers= response->ToString();
  std::size_t pos = headers.find("\r\n\r\n");
  std::string h_str = headers.substr(0, pos+1);
  std::cout << h_str<< std::endl;
  return RequestHandler::OK;
}
