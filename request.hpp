#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <vector>
#include <memory>


// Represents an HTTP Request.
//
// Usage:
//   auto request = Request::Parse(raw_request);

class Request {
 public:
  static std::unique_ptr<Request> Parse(const std::string& raw_request);

  std::string raw_request() const;
  std::string method() const;
  std::string uri() const;
  std::string version() const;

  using Headers = std::vector<std::pair<std::string, std::string>>;
  Headers headers() const;

  std::string body() const;

 private:
  std::vector<std::pair<std::string, std::string>> _headers;
  std::string _raw_request;
  std::string _method;
  std::string _uri;
  std::string _version;
  std::string _body;

};

#endif //REQUEST_HPP
