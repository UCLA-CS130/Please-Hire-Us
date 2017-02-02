#include <string>
#include <iostream>

class HttpRequest {
  public:
    HttpRequest(std::string responseCode, std::string contentType, std::string msgBody):responseCode(responseCode),contentType(contentType),msgBody(msgBody){};
  
    std::string toString(){
      std::string msg = "HTTP/1.1 " + responseCode + " OK\nContent-Type:" + contentType + "\n\n" + msgBody + "\n\n";
      return msg;
    }
  private:
    std::string responseCode;
    std::string contentType;
    std::string msgBody;


};