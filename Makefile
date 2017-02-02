#Makefile

CCX = g++

#Compiler Flags
CXXFLAGS= -Wall -std=c++11

#Link Flags
LFLAGS= -lboost_system

SRC = echo_server_main.cpp echo_server.cpp config_parser.cc

echo_server: echo_server.hpp config_parser.h
	$(CCX) -o $@ $^ $(CXXFLAGS) $(SRC) $(LFLAGS)

clean:
	rm -rf .o* echo_server
