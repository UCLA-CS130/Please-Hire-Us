#ifndef REVERSE_PROXY_HPP
#define REVERSE_PROXY_HPP

#include <string>
#include "request.hpp"
#include "response.hpp"
#include "request_handler.hpp"

/// The common handler for all incoming requests.
class ReverseProxyHandler: public RequestHandler
{
public:
  ReverseProxyHandler();
  Status Init(const std::string& uri_prefix,
                      const NginxConfig& config);
  Status HandleRequest(const Request& request,
                               Response* response);
  RequestHandler::Status SendProxyRequest(const std::string& request_string, const std::string& new_host, Response* response,
                                                      const int& attempt_num = 0);
  std::pair<std::string, std::string> ProcessHeaderLine(std::string header);
  bool ParseRedirect(std::istream& response_stream, std::string& redirect_URI,
                                        std::string& redirect_host);
  std::string ParseBody(boost::asio::streambuf* response_buf);
  std::string ParseBody(std::string);
private:

  std::string m_remote_host;
  std::string m_remote_port;
  std::string m_uri_prefix;
  std::string m_path;
};


//REGISTER_REQUEST_HANDLER(ReverseProxyHandler);
#endif // REVERSE_PROXY_HPP