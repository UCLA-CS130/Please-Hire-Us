#Makefile

CCX = g++


#Test Flags
GTEST_DIR = googletest/googletest
GTEST_FLAGS = -isystem ${GTEST_DIR}/include 
TEST_CCXFLAGS = -std=c++0x -Wall -Werror -pthread -fprofile-arcs -ftest-coverage
TEST_SRC = server_test.cc server.cpp

#Compiler Flags
CXXFLAGS= -Wall -std=c++11

#Link Flags
LFLAGS= -lboost_system

SRC = server_main.cpp server.cpp httpRequest.cpp httpResponse.cpp request_handler.cpp request_handler_echo.cpp config_parser.cc
DEP = server.hpp config_parser.h httpRequest.hpp httpResponse.hpp request_handler.hpp request_handler_echo.hpp

default: server

server: $(DEP)
	$(CCX) -o $@ $^ $(CXXFLAGS) $(SRC) $(LFLAGS)

test:
	$(CCX) $(TEST_CCXFlAGS) $(GTEST_FLAGS) -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc 
	ar -rv libgtest.a gtest-all.o
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) server_test.cc config_parser.cc httpResponse.cpp httpRequest.cpp request_handler.cpp request_handler_echo.cpp server.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o server_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o config_parser_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_echo_test.cc httpResponse.cpp httpRequest.cpp request_handler.cpp request_handler_echo.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_handler_echo_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) httpResponse_test.cc httpResponse.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o httpResponse_test
	./config_parser_test
	./server_test
	./request_handler_echo_test
	./httpResponse_test

coverage:
	$(CCX) $(TEST_CCXFlAGS) $(GTEST_FLAGS) -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc 
	ar -rv libgtest.a gtest-all.o
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) server_test.cc config_parser.cc httpResponse.cpp httpRequest.cpp request_handler.cpp request_handler_echo.cpp server.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o server_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o config_parser_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_echo_test.cc httpResponse.cpp httpRequest.cpp request_handler.cc request_handler_echo.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_handler_echo_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) httpResponse_test.cc httpResponse.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o httpResponse_test
	./config_parser_test
	gcov -r config_parser.cc
	./server_test
	gcov -r server.cpp
	./request_handler_echo_est.cpp
	gcov -r httpRequest.cpp
	./httpResponse_test
	gcov -r httpResponse.cpp


clean:
	rm -f *.o *_test *.gcov *.gcno *.gcda server
