#include <iostream>
#include "server.hpp"

int main(int argc, char *argv[]){
  
  if (argc != 2){
    std::cerr << "Please provide a single config file." << std::endl;
    return -1;
  }

  char * configFile = argv[1];

  
  // Ensure that input config is valid
  NginxConfig config_;
  NginxConfigParser parser_;
  bool didParse = parser_.Parse(configFile, &config_);
  if (!didParse){
    std::cerr << "Please ensure the input config file specified exists and is valid." << std::endl;
    return -1;
  }

  Server server(config_);

  std::string errorMessage;
  bool successInit = server.init(errorMessage);
  if (!successInit){
    std::cerr << "Server.init failed with error message: " << errorMessage;
    return -1;
  }

  server.run(); 
}
