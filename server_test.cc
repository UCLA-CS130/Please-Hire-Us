#include "gtest/gtest.h"
#include "server.hpp"
#include "config_parser.h"
#include <string>


// Fixture to be used for testing the Server config
class ServerConfigTest : public :: testing::Test {
  protected:
    std::string inputConfig_;
    std::string errorMessage_;
    NginxConfig server_config_;  

    //Wrapper function to parse config before initializing server
    bool parse (std::string config_, NginxConfig& server_config_){
      NginxConfigParser parser_;
      std::stringstream config_stream(config_);
      return parser_.Parse(&config_stream, &server_config_);
    }
};

TEST_F(ServerConfigTest, SimpleConfig){
  inputConfig_ = 
  "server {"
  "  path /static StaticHandler {"
  "    root '/example';"
  "  }"
  "  echo /echo EchoHandler {"
  "    root '/fix/this';"
  "  }"
  "  port 3000;"
  "}";
  
  bool didParse = parse(inputConfig_, server_config_);
  ASSERT_TRUE(didParse); 

  Server server(server_config_); 
  bool success = server.extractConfig(errorMessage_);
  EXPECT_TRUE(success);
  
  EXPECT_EQ(server.getPort(), 3000);
  
  EXPECT_TRUE(server.init(errorMessage_));
}


TEST_F(ServerConfigTest, BadPort){
  inputConfig_ =
  "server {"
  "  port -4300;"
  "}";

  bool didParse = parse(inputConfig_, server_config_);
  ASSERT_TRUE(didParse);
 
  Server server(server_config_);
  bool failure = server.extractConfig(errorMessage_);
  EXPECT_FALSE(failure);

  std::string empty = "";
  EXPECT_STRNE(empty.c_str(), errorMessage_.c_str());

  EXPECT_FALSE(server.init(errorMessage_));
}

TEST_F(ServerConfigTest, NestedConfig){
  inputConfig_ =
  "server {"
  "  path /static StaticHandler {"
  "    root '/var/www/';"
  "  }"
  "  listen 8000;"
  "}";

  bool didParse = parse(inputConfig_, server_config_);
  ASSERT_TRUE(didParse);

  Server server(server_config_);
  bool success = server.extractConfig(errorMessage_);
  EXPECT_TRUE(success);

  EXPECT_EQ(8000, server.getPort());

  EXPECT_TRUE(server.init(errorMessage_));
}

