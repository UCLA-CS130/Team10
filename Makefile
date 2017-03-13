all: release

CPPFLAGS+=-std=c++11 -Wall
CPPFLAGS+=-g

CPPFLAGS+=-static-libgcc -static-libstdc++ -pthread
LDFLAGS+=-Wl,-Bstatic -lboost_system -lboost_thread -lboost_regex

UNIT_TESTS_DIR=unit_tests
GMOCK_DIR=googletest/googlemock
GTEST_DIR=googletest/googletest

TESTS=unit_tests/config_parser_test unit_tests/request_test unit_tests/static_handler_test  unit_tests/response_test unit_tests/connection_manager_test unit_tests/mime_types_test unit_tests/connection_test unit_tests/echo_handler_test unit_tests/not_found_handler_test unit_tests/reverse_proxy_handler_test

TEST_CLASSES=config_parser.cpp connection.cpp connection_manager.cpp server_config.cpp mime_types.cpp response.cpp echo_handler.cpp server.cpp request.cpp not_found_handler.cpp static_handler.cpp request_handler.cpp status_handler.cpp reverse_proxy_handler.cpp log.cpp markdown.cpp markdown-tokens.cpp

CLASSES=config_parser.o connection.o connection_manager.o server_config.o main.o mime_types.o response.o echo_handler.o server.o request.o not_found_handler.o static_handler.o request_handler.o status_handler.o reverse_proxy_handler.o log.o markdown.o markdown-tokens.o

.PHONY: clean test release

int:  ;	 python test.py

debug: webserver
debug: test
	for test in $(TEST_CLASSES); do gcov -r $$test; done

release: CPPFLAGS+=-O3
release: webserver

libgtest.a:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

libgmock.a:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -I${GMOCK_DIR} -I${GTEST_DIR} -pthread -c ${GMOCK_DIR}/src/gmock-all.cc
	ar -rv libgmock.a gmock-all.o

%_test: %_test.cpp libgtest.a libgmock.a
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread $(TEST_CLASSES) $(TESTS:=.cpp) ${GTEST_DIR}/src/gtest_main.cc libgtest.a -fprofile-arcs -ftest-coverage -o $@ -lboost_system -lboost_thread -lboost_regex

server_test: $(UNIT_TESTS_DIR)/server_test.cpp libgtest.a libgmock.a
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -pthread $(UNIT_TESTS_DIR)/server_test.cpp server.cpp ${GMOCK_DIR}/src/gmock_main.cc libgtest.a libgmock.a -fprofile-arcs -ftest-coverage -o server_test -lboost_system -lboost_thread -lboost_regex

test: $(TESTS)
	for test in $(TESTS); do ./$$test; done

webserver: $(CLASSES)
	$(CXX) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf *.o *.a *.gcno *.gcda *.gcov gtest* webserver *_test deploy/

dist:	;
	./deploy.sh
