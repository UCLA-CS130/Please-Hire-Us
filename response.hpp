#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <memory>
#include <vector>

// For the Request and Response classes, you need to implement the methods
// and add private data as appropriate. You may also need to modify or extend
// the API when implementing the reverse proxy. Use your good judgment.

// Represents an HTTP response.
//
// Usage:
//   Response r;
//   r.SetStatus(RESPONSE_200);
//   r.SetBody(...);
//   return r.ToString();
//
// Constructed by the RequestHandler, after which the server should call ToString
// to serialize.
class Response {
 public:
  enum ResponseCode {
    // Define your HTTP response codes here.
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    UNSUPPORTED_MEDIA_TYPE = 415
  };
  
  void SetStatus(const ResponseCode response_code);
  void AddHeader(const std::string& header_name, const std::string& header_value);
  void SetBody(const std::string& body);
  
  std::string ToString();
 private:
  std::vector<std::pair<std::string, std::string>> _headers;
  ResponseCode _response_code;
  std::string _body;
};

#endif //RESPONSE_HPP
