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
#LDFLAGS= -static-libgcc -static-libstdc++ -Wl,-Bstatic -lboost_system -lboost_regex -lpthread
LDFLAGS = -static-libgcc -static-libstdc++ -lpthread -Wl,-Bstatic -lboost_log_setup -lboost_log -lboost_thread -lboost_system -lm -lboost_regex

SRC = server_main.cpp server.cpp request.cpp response.cpp request_handler_echo.cpp request_handler_static.cpp request_handler_notFound.cpp request_handler_status.cpp markdown-lib/markdown.cpp markdown-lib/markdown-tokens.cpp 
CPP_OBJ = server_main.o server.o request.o response.o request_handler_echo.o request_handler_static.o request_handler_notFound.o request_handler_status.o request_handler_reverse_proxy.o markdown-lib/markdown.o markdown-lib/markdown-tokens.o
CC_OBJ = config_parser.o

KEY_LOC=../aws-key-pairs/cs130-ec2-key-pair.pem
EC2_HOST=ec2-35-162-66-89.us-west-2.compute.amazonaws.com
EC2_SERVER=ec2-user@ec2-35-162-66-89.us-west-2.compute.amazonaws.com


default: server

$(CPP_OBJ): %.o : %.cpp 
	$(CCX) $(CXXFLAGS) -c -o $@ $<
$(CC_OBJ): config_parser.cc config_parser.h
	$(CCX) $(CXXFLAGS) -c config_parser.cc

server: $(CPP_OBJ) $(CC_OBJ)
	$(CCX) -o $@ $^ $(CXXFLAGS) $(OBJ) $(LDFLAGS)

test:
	$(CCX) $(TEST_CCXFlAGS) $(GTEST_FLAGS) -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc 
	ar -rv libgtest.a gtest-all.o
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) server_test.cc config_parser.cc response.cpp request.cpp request_handler.hpp request_handler_echo.cpp request_handler_static.cpp request_handler_status.cpp request_handler_reverse_proxy.cpp request_handler_notFound.cpp server.cpp markdown-lib/markdown.cpp markdown-lib/markdown-tokens.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o server_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o config_parser_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_echo_test.cc response.cpp request.cpp request_handler_echo.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_handler_echo_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_static_test.cc config_parser.cc response.cpp request.cpp request_handler_static.cpp markdown-lib/markdown.cpp markdown-lib/markdown-tokens.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_handler_static_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_status_test.cc config_parser.cc server.cpp request_handler_reverse_proxy.cpp response.cpp request.cpp request_handler_status.cpp request_handler_echo.cpp request_handler_static.cpp request_handler_notFound.cpp markdown-lib/markdown.cpp markdown-lib/markdown-tokens.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_handler_status_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_notFound_test.cc response.cpp request.cpp request_handler_notFound.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_handler_notFound_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) response_test.cc response.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o response_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_test.cc request.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_test
	$(CXX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_reverse_proxy_test.cc request.cpp response.cpp request_handler_reverse_proxy.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_handler_reverse_proxy_test
	./config_parser_test
	./server_test
	./request_handler_echo_test
	./request_handler_static_test
	./request_handler_status_test
	./request_handler_notFound_test
	./response_test
	./request_test	
	./request_handler_reverse_proxy_test

reverse_proxy_int_test:
	python reverse_proxy_test.py

coverage:
	$(CCX) $(TEST_CCXFlAGS) $(GTEST_FLAGS) -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc 
	ar -rv libgtest.a gtest-all.o
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) server_test.cc config_parser.cc response.cpp request.cpp request_handler_echo.cpp request_handler_reverse_proxy.cpp request_handler_static.cpp request_handler_status.cpp request_handler_reverse_proxy.cpp request_handler_notFound.cpp server.cpp markdown-lib/markdown.cpp markdown-lib/markdown-tokens.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o server_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o config_parser_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_echo_test.cc response.cpp request.cpp request_handler_echo.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_handler_echo_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_static_test.cc config_parser.cc response.cpp request.cpp request_handler_static.cpp markdown-lib/markdown.cpp markdown-lib/markdown-tokens.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_handler_static_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_status_test.cc config_parser.cc request_handler_reverse_proxy.cpp server.cpp response.cpp request.cpp request_handler_status.cpp request_handler_echo.cpp request_handler_static.cpp request_handler_notFound.cpp markdown-lib/markdown.cpp markdown-lib/markdown-tokens.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_handler_status_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_notFound_test.cc response.cpp request.cpp request_handler_notFound.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_handler_notFound_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) response_test.cc response.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o response_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_test.cc request.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_test
	$(CCX) $(TEST_CCXFLAGS) $(GTEST_FLAGS) request_handler_reverse_proxy_test.cc request_handler_reverse_proxy.cpp response.cpp request.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LDFLAGS) -o request_handler_reverse_proxy_test
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
	./request_handler_reverse_proxy_test
	gcov -r request_handler_reverse_proxy.cpp

# docker building and deployment scripts created with help from the Makefile of Team13, spaceteam, and Team22, as well as http://stackoverflow.com/questions/23935141/how-to-copy-docker-images-from-one-host-to-another-without-via-repository
docker_build:
	docker build -t httpserver.build .
	docker run httpserver.build > ./deploy/binary.tar
	tar -xvf ./deploy/binary.tar -C ./deploy
	rm ./deploy/binary.tar
	docker build -t httpserver -f deploy/Dockerfile.run ./deploy

docker_deploy:
	docker save -o httpserver-image httpserver
	scp -i $(KEY_LOC) httpserver-image $(EC2_SERVER):~
	rm httpserver-image
	ssh -i $(KEY_LOC) $(EC2_SERVER) 'docker load -i httpserver-image; docker kill `docker ps -q`; docker run --rm -t -p 80:8080 httpserver; exit'

docker_kill:
	ssh -i $(KEY_LOC) $(EC2_SERVER) 'docker kill `docker ps -q`'

clean:
	rm -f *.o *_test *.gcov *.gcno *.gcda server
