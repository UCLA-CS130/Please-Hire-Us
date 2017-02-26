#Makefile

CCX = g++

#Test Flags
GTEST_DIR = googletest/googletest
GTEST_FLAGS = -isystem ${GTEST_DIR}/include 
TEST_CCXFLAGS = -std=c++11 -pthread -fprofile-arcs -ftest-coverage
TEST_SRC = server_test.cc server.cpp

#Compiler Flags
CXXFLAGS= -std=c++11

#Link Flags
LFLAGS= -lboost_system -lboost_regex

SRC = server_main.cpp server.cpp request.cpp response.cpp request_handler_echo.cpp request_handler_static.cpp request_handler_notFound.cpp request_handler_status.cpp
CPP_OBJ = server_main.o server.o request.o response.o request_handler_echo.o request_handler_static.o request_handler_notFound.o request_handler_status.o
CC_OBJ = config_parser.o

default: server

$(CPP_OBJ): %.o : %.cpp 
	$(CCX) $(CXXFLAGS) -c -o $@ $<
$(CC_OBJ): config_parser.cc config_parser.h
	$(CCX) $(CXXFLAGS) -c config_parser.cc

server: $(CPP_OBJ) $(CC_OBJ)
	$(CCX) -o $@ $^ $(CXXFLAGS) $(OBJ) $(LFLAGS)

test:
	$(CCX) $(TEST_CCXFlAGS) $(GTEST_FLAGS) -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc 
	ar -rv libgtest.a gtest-all.o
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) server_test.cc config_parser.cc response.cpp request.cpp request_handler_echo.cpp request_handler_static.cpp request_handler_status.cpp request_handler_notFound.cpp server.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o server_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o config_parser_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_echo_test.cc response.cpp request.cpp request_handler_echo.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_handler_echo_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_static_test.cc config_parser.cc response.cpp request.cpp request_handler_static.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_handler_static_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_status_test.cc config_parser.cc server.cpp response.cpp request.cpp request_handler_status.cpp request_handler_echo.cpp request_handler_static.cpp request_handler_notFound.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_handler_status_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_notFound_test.cc response.cpp request.cpp request_handler_notFound.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_handler_notFound_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) response_test.cc response.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o response_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_test.cc request.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_test
	./config_parser_test
	./server_test
	./request_handler_echo_test
	./request_handler_static_test
	./request_handler_status_test
	./request_handler_notFound_test
	./response_test
	./request_test	

coverage:
	$(CCX) $(TEST_CCXFlAGS) $(GTEST_FLAGS) -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc 
	ar -rv libgtest.a gtest-all.o
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) server_test.cc config_parser.cc response.cpp request.cpp request_handler_echo.cpp request_handler_static.cpp request_handler_status.cpp request_handler_notFound.cpp server.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o server_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o config_parser_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_echo_test.cc response.cpp request.cpp request_handler_echo.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_handler_echo_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_static_test.cc config_parser.cc response.cpp request.cpp request_handler_static.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_handler_static_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_status_test.cc config_parser.cc server.cpp response.cpp request.cpp request_handler_status.cpp request_handler_echo.cpp request_handler_static.cpp request_handler_notFound.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_handler_status_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_notFound_test.cc response.cpp request.cpp request_handler_notFound.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_handler_notFound_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) response_test.cc response.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o response_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_test.cc request.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LFLAGS) -o request_test
	./config_parser_test
	gcov -r config_parser.cc
	./server_test
	gcov -r server.cpp
	./request_handler_echo_test
	gcov -r request_handler_echo.cpp
	./request_handler_static_test
	gcov -r request_handler_static.cpp
	./request_handler_status_test
	gcov -r request_handler_status.cpp
	./request_handler_notFound_test
	gcov -r request_handler_notFound.cpp
	./response_test
	gcov -r response.cpp
	./request_test
	gcov -r request.cpp


clean:
	rm -f *.o *_test *.gcov *.gcno *.gcda server
