#ifndef http_request
#define http_request

#include <string>
#include <boost/asio.hpp>

class HttpRequest {
public:
  HttpRequest(std::string inputRequest);
  bool parse();
  std::string getPath() const;
  std::string getMethod() const; 
  std::string getRequest() const;
  bool isEcho() const;
  
private:
  std::string _method;
  std::string _path; 
  std::string _request;
  bool _isEcho; 

};

#endif
