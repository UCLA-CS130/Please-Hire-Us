#Makefile

CCX = g++

#Flag
CXXFLAGS= -Wall -std=c++11

SRC = echo_server_main.cpp echo_server.cpp config_parser.cc

echo_server:
	$(CXX) -o $@ $^ $(CXXFLAGS) $(SRC)

clean:
	rm -rf .o* echo_server
