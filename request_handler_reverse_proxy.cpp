#include "request_handler_reverse_proxy.hpp"
#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

ReverseProxyHandler::ReverseProxyHandler()
{}

RequestHandler::Status ReverseProxyHandler::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
  m_uri_prefix = uri_prefix;
  m_remote_host = "";
  m_remote_port = "http";
  m_path = "";
  std::string url_ = "";

  for (auto statement : config.statements_)
	{
		if(statement->tokens_.size() == 2 && statement->tokens_[0] == "remote_host")
		{
			url_ = statement->tokens_[1];

			std::string::size_type protocol_pos = url_.find("//");
			if(protocol_pos == std::string::npos) {
				std::cout << "remote_host didn't specify protocol.'" << std::endl;
				return RequestHandler::INVALID;
			}
			m_remote_port = url_.substr(0, protocol_pos - 1);

			std::string::size_type host_pos = url_.find('/', protocol_pos + 2);
			if(host_pos != std::string::npos) {
				m_remote_host = url_.substr(protocol_pos + 2, host_pos - protocol_pos - 2);
				m_path = url_.substr(host_pos);
			}
			else {
				m_remote_host = url_.substr(protocol_pos + 2);
				m_path = "/";
			}
			printf("m_path: %s\n", m_path.c_str());
			return RequestHandler::OK;
		}
	}

  // Need both the host and the port config values
  if (m_remote_host.empty() || m_remote_port.empty())
  {
    return RequestHandler::INVALID;
  }

  return RequestHandler::OK;
}

RequestHandler::Status ReverseProxyHandler::HandleRequest(const Request& request,
                               Response* response)
{
  // Get the request URI (e.g. /proxy/static/file.txt)
  std::string new_uri = request.uri();
  std::cout << "Original URI: " << new_uri << std::endl;

  // Generate new URI (/static/file.txt)
  if (m_uri_prefix != "/")
  {
    if (new_uri.find(m_uri_prefix) != 0)
    {
        // Something is wrong, the prefix does not match the URI.
        response = nullptr;
        return RequestHandler::INVALID;
    }
    else
    {
        // Remove the prefix from the URI to get the file name.
        new_uri.erase(0, m_uri_prefix.length());
    }
  }
  if (new_uri == "")
  {
    new_uri = m_path;
  }
  std::cout << "New URI: " << new_uri << std::endl;
  // Generate new request
  //    GET new_URI HTTP/1.0\r\n\r\n
  std::string new_request = "GET " + new_uri + " HTTP/1.0\r\n\r\n";

  std::cout << "Send Proxy Request...\n";
  return SendProxyRequest(new_request, m_remote_host, response);
}


RequestHandler::Status ReverseProxyHandler::SendProxyRequest(const std::string& request_string, const std::string& new_host, 
                                                            Response* response, const int& attempt_num)
{
  // Create connection to m_remote_host:m_remote_port
  std::cout << "Create connection...\n";
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query(new_host, m_remote_port);
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  boost::asio::ip::tcp::resolver::iterator end;

  boost::asio::ip::tcp::socket socket(io_service);
  boost::system::error_code error = boost::asio::error::host_not_found;
  // There might be multiple endpoints. Connect to the first one that works.
  while (error && endpoint_iterator != end)
  {
    socket.close();
    socket.connect(*endpoint_iterator++, error);
  }

  // Send request to m_remote_host:m_remote_port
  std::cout << "Writing to socket...\n";
  std::cout << request_string << std::endl;
  boost::asio::write(socket, boost::asio::buffer(request_string));

  boost::asio::streambuf response_buf;
  // Read entire response
  boost::system::error_code ec;
  std::string response_string;
	std::size_t bytes_read;
  while((bytes_read = boost::asio::read(socket, response_buf, 
							 boost::asio::transfer_at_least(1), ec))) {
                
		// Read the values form the buffer (all bytes read)
		std::string read_data = std::string(boost::asio::buffers_begin(response_buf.data()),
											boost::asio::buffers_begin(response_buf.data()) + bytes_read);
		response_string += read_data;
		// Remove the bytes read from the buffer
		response_buf.consume(bytes_read);
	}
  if(ec != boost::asio::error::eof) {
		// Error reading.
		std::cout << "Failed to read response..." << std::endl;;
		return RequestHandler::INVALID;
	}

  // Check that response is OK. 
  // Adapted from http://www.boost.org/doc/libs/1_57_0/doc/html/boost_asio/example/cpp03/http/client/sync_client.cpp
  std::stringstream response_stream(response_string);
  std::string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message);
  std::cout << "Status Message: " << status_message << std::endl;
  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
  {
    std::cout << "HTTP Version: " << http_version << std::endl;
    std::cerr << "Invalid response\n";
    return RequestHandler::INVALID;
  }
  if (status_code == Response::MOVED_TEMPORARILY)
  {
    std::cout << "Redirect attempt number " << attempt_num << std::endl;
    // Check attempt_num. If >= 5, then we've been redirected too many times.
    if (attempt_num >= 5)
    {
      response = nullptr;
      return RequestHandler::INVALID;
    }
    // Read the headers until we find the location
    // Read the response headers, which are terminated by a blank line.
    std::string redirect_URI = "";
    std::string redirect_host = "";
    bool parsed_redirect = ParseRedirect(response_stream, redirect_URI, redirect_host);
    if (parsed_redirect == false)
    {
      return RequestHandler::INVALID;
    }
    else
    {
      std::string redirect_request =  "GET " + redirect_URI + " HTTP/1.0\r\n\r\n";
      // Send proxy request to new location
      return SendProxyRequest(redirect_request, redirect_host, response, attempt_num + 1);
    }
  }
  else if (status_code != Response::OK)
  {
    return RequestHandler::INVALID;
  }

  // Process the response headers.
  std::string header;
  while (std::getline(response_stream, header) && header != "\r")
  {
    // Get the header attribute
    // Get header value
    // Set header attribute/value in response
    if (!header.empty()) {
      std::pair<std::string, std::string> parsed_header = ProcessHeaderLine(header);
      if (parsed_header.first != "" && parsed_header.first != "Content-Length" && parsed_header.second != "")
      {
        response->AddHeader(parsed_header.first, parsed_header.second);
      }
    }
  }
  //response->AddHeader("Content-Type", "application/octet-stream");

  std::string::size_type body_pos = response_string.find("\r\n\r\n");
  if (body_pos == std::string::npos) {
    std::cout << "Error: empty body..." << std::endl;
    return RequestHandler::INVALID;
  }
  std::string response_body = response_string.substr(body_pos + 4);

  std::string parsed_body = ParseBody(response_body);
  response->SetStatus(Response::OK);
  response->SetBody(parsed_body);
  return RequestHandler::OK;
}

std::pair<std::string, std::string> ReverseProxyHandler::ProcessHeaderLine(std::string header)
{
  std::string header_field = "";
  std::string header_value = "";
  std::size_t field_index = header.find_first_of(":");

  if (field_index != std::string::npos) {
    header_field = header.substr(0, field_index);
    header_value = header.substr(field_index+2, std::string::npos);
    //delete carriage return
    header_value = header_value.substr(0, header_value.length()-1);
  }

  return std::make_pair( header_field, header_value);
}

bool ReverseProxyHandler::ParseRedirect(std::istream& response_stream, std::string& redirect_URI,
                                        std::string& redirect_host)
{
  std::string header;
  while (std::getline(response_stream, header) && header != "\r")
  {
    // Find the new location header
    if (!header.empty())
    {
      std::pair<std::string, std::string> parsed_header = ProcessHeaderLine(header);
      boost::algorithm::to_lower(parsed_header.first);
      if ( parsed_header.first == "location")
      {
        std::string redirect_URL = parsed_header.second;
        std::cout << "Redirect to " << redirect_URL << std::endl;

        // Generate new host
        size_t host_start = redirect_URL.find("//");
        if (host_start == std::string::npos || host_start + 2 >= redirect_URL.length())
        {
          // There is no valid URL?
          std::cerr << "No valid URL." << std::endl;
          return false;
        }
        else
        {
          // Parse the URL to get the host and the new URI
          redirect_URL = redirect_URL.substr(host_start + 2);
          size_t host_end = redirect_URL.find("/");

          redirect_host = redirect_URL.substr(0, host_end);
          redirect_URI = redirect_URL.substr(host_end);
          if (redirect_URI == "")
          {
            redirect_URI = "/";
          }
          return true;
        }
      }
    }
  }

  std::cerr << "Could not find redirect location." << std::endl;
  return false;
}

std::string ReverseProxyHandler::ParseBody(boost::asio::streambuf* response_buf)
{
  boost::asio::streambuf::const_buffers_type response_body = response_buf->data();
  std::string body(boost::asio::buffers_begin(response_body), boost::asio::buffers_begin(response_body) + response_buf->size());

  // Modify response (prepend uri_prefix to beginning of all href and src attributes)
  if (m_uri_prefix != "/") {
    std::string new_href = "href=\"" + m_uri_prefix + "/";
    boost::replace_all(body, "href=\"/", new_href);
    std::string new_src = "src=\"" + m_uri_prefix + "/";
    boost::replace_all(body, "src=\"/", new_src);
  }

  return body;
}

std::string ReverseProxyHandler::ParseBody(std::string body)
{
  // Modify response (prepend uri_prefix to beginning of all href and src attributes)
  if (m_uri_prefix != "/") {
    std::string new_href = "href=\"" + m_uri_prefix + "/";
    boost::replace_all(body, "href=\"/", new_href);
    std::string new_src = "src=\"" + m_uri_prefix + "/";
    boost::replace_all(body, "src=\"/", new_src);
  }

  return body;
}