#Makefile

CCX = g++


#Test Flags
GTEST_DIR = googletest/googletest
GTEST_FLAGS = -isystem ${GTEST_DIR}/include 
TEST_CCXFLAGS = -std=c++0x -Wall -Werror -pthread -fprofile-arcs -ftest-coverage
TEST_SRC = echo_server_test.cc echo_server.cpp

#Compiler Flags
CXXFLAGS= -Wall -std=c++11

#Link Flags
LFLAGS= -lboost_system

SRC = echo_server_main.cpp echo_server.cpp httpRequest.cpp httpResponse.cpp config_parser.cc

echo_server: echo_server.hpp config_parser.h
	$(CCX) -o $@ $^ $(CXXFLAGS) $(SRC) $(LFLAGS)

test:
	$(CCX) $(TEST_CCXFlAGS) $(GTEST_FLAGS) -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc 
	ar -rv libgtest.a gtest-all.o
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) echo_server_test.cc config_parser.cc httpResponse.cpp httpRequest.cpp echo_server.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o echo_server_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o config_parser_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) httpRequest_test.cc httpResponse.cpp httpRequest.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o httpRequest_test
	./config_parser_test
	./echo_server_test
	./httpRequest_test

coverage:
	$(CCX) $(TEST_CCXFlAGS) $(GTEST_FLAGS) -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc 
	ar -rv libgtest.a gtest-all.o
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) echo_server_test.cc config_parser.cc httpResponse.cpp httpRequest.cpp echo_server.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o echo_server_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o config_parser_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) httpRequest_test.cc httpResponse.cpp httpRequest.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o httpRequest_test
	./config_parser_test
	gcov -r config_parser.cc
	./echo_server_test
	gcov -r echo_server.cpp
	./httpRequest_test
	gcov -r httpRequest.cpp


clean:
	rm -f *.o *_test *.gcov *.gcno *.gcda echo_server
