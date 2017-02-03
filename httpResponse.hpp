#ifndef http_response
#define http_response
#include<string>


class HttpResponse {
public:
  HttpResponse(std::string responseCode, std::string contentType, std::string msgBody);
  bool checkValidity();
  std::string toString();

private:
  std::string responseCode;
  std::string contentType;
  std::string msgBody;
};

#endif
