#include "config_parser.h"

class EchoServer {
 public:
   EchoServer(NginxConfig inputConfig);
   void run();

 private:
   NginxConfig config_;
   int port;
};
