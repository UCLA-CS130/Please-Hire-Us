#include "echo_server.hpp"

EchoServer::EchoServer(NginxConfig inputConfig) :config_(inputConfig){
  //TODO: Don't hardcode the port extraction as done below
  std::string str_port = config_.statements_[0]->child_block_->statements_[0]->tokens_[1];
  port = std::stoi(str_port);  // May just be able to leave this as a string
  
  std::cout << "Initialized server with port: " << port << std::endl;
}

void EchoServer::run(){
  std::cout << "running server" << std::endl;
}
