#ifndef http_request
#define http_request

#include <string>

class HttpRequest {
public:
  HttpRequest(std::string inputRequest);
  void parse();

  std::string request;
  std::string root;
  std::string method;
  std::string URI;
  std::string connection;
  std::string file_type;

};

#endif
