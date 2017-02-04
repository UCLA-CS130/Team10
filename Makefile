all: clean release

CPPFLAGS+=-std=c++11 -Wall
CPPFLAGS+=-g

CPPFLAGS+=-pthread
LDFLAGS+=-lboost_system

UNIT_TESTS_DIR=unit_tests
GMOCK_DIR=googletest/googlemock
GTEST_DIR=googletest/googletest

TESTS=unit_tests/config_parser_test unit_tests/request_test unit_tests/request_handler_test unit_tests/request_parser_test unit_tests/reply_test unit_tests/connection_manager_test unit_tests/mime_types_test unit_tests/connection_test

TEST_CLASSES=config_parser.cpp connection.cpp connection_manager.cpp mime_types.cpp reply.cpp request_handler.cpp request_parser.cpp server.cpp

CLASSES=config_parser.o connection.o connection_manager.o main.o mime_types.o reply.o request_handler.o request_parser.o server.o

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
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread $(TEST_CLASSES) $(TESTS:=.cpp) ${GTEST_DIR}/src/gtest_main.cc libgtest.a -fprofile-arcs -ftest-coverage -o $@ -lboost_system

server_test: $(UNIT_TESTS_DIR)/server_test.cpp libgtest.a libgmock.a
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -pthread $(UNIT_TESTS_DIR)/server_test.cpp server.cpp ${GMOCK_DIR}/src/gmock_main.cc libgtest.a libgmock.a -fprofile-arcs -ftest-coverage -o server_test -lboost_system

test: $(TESTS)
	for test in $(TESTS); do ./$$test; done

webserver: $(CLASSES)
	$(CXX) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o *.a *.gcno *.gcda *.gcov gtest* webserver *_test
