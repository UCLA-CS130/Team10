all: clean release

CPPFLAGS+=-std=c++11 -Wall
CPPFLAGS+=-g

CPPFLAGS+=-pthread
LDFLAGS+=-lboost_system

UNIT_TESTS_DIR=unit_tests
GMOCK_DIR=googletest/googlemock
GTEST_DIR=googletest/googletest

TESTS=config_parser_test request_test request_handler_test request_parser_test reply_test connection_test connection_manager_test mime_types_test server_test

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

config_parser_test: $(UNIT_TESTS_DIR)/config_parser_test.cpp libgtest.a
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread config_parser.cpp $(UNIT_TESTS_DIR)/config_parser_test.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -fprofile-arcs -ftest-coverage -o config_parser_test

request_test: $(UNIT_TESTS_DIR)/request_test.cpp libgtest.a
	 g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread $(UNIT_TESTS_DIR)/request_test.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_test -lboost_system


request_handler_test: $(UNIT_TESTS_DIR)/request_handler_test.cpp request_handler.cpp reply.cpp libgtest.a mime_types.cpp libgtest.a
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread $(UNIT_TESTS_DIR)/request_handler_test.cpp reply.cpp request_handler.cpp mime_types.cpp  ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handler_test -lboost_system

request_parser_test: $(UNIT_TESTS_DIR)/request_parser_test.cpp libgtest.a
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread $(UNIT_TESTS_DIR)/request_parser_test.cpp request_parser.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_parser_test -lboost_system

reply_test: $(UNIT_TESTS_DIR)/reply_test.cpp reply.cpp libgtest.a
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread $(UNIT_TESTS_DIR)/reply_test.cpp reply.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o reply_test -lboost_system


connection_test: $(UNIT_TESTS_DIR)/connection_test.cpp connection.cpp reply.cpp request_handler.cpp request_parser.cpp mime_types.cpp connection_manager.cpp libgtest.a 
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread $(UNIT_TESTS_DIR)/connection_test.cpp connection.cpp reply.cpp request_handler.cpp request_parser.cpp mime_types.cpp connection_manager.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o connection_test -lboost_system

connection_manager_test: $(UNIT_TESTS_DIR)/connection_manager_test.cpp connection.cpp reply.cpp request_handler.cpp request_parser.cpp mime_types.cpp connection_manager.cpp libgtest.a
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread $(UNIT_TESTS_DIR)/connection_manager_test.cpp connection.cpp reply.cpp request_handler.cpp request_parser.cpp mime_types.cpp connection_manager.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o connection_manager_test -lboost_system

mime_types_test: $(UNIT_TESTS_DIR)/mime_types_test.cpp mime_types.cpp libgtest.a
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread $(UNIT_TESTS_DIR)/mime_types_test.cpp mime_types.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o mime_types_test

server_test: $(UNIT_TESTS_DIR)/server_test.cpp connection.cpp reply.cpp request_handler.cpp request_parser.cpp mime_types.cpp connection_manager.cpp server.cpp libgtest.a libgmock.a
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -pthread $(UNIT_TESTS_DIR)/server_test.cpp server.cpp connection.cpp request_parser.cpp mime_types.cpp reply.cpp request_handler.cpp connection_manager.cpp  ${GMOCK_DIR}/src/gmock_main.cc libgtest.a libgmock.a -o server_test -lboost_system

#%.o: $(CLASSES)
#	$(CXX) -Werror $(CPPFLAGS) $^ -c

test: $(TESTS)
	for test in $(TESTS); do ./$$test; done

webserver: $(CLASSES)
	$(CXX) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o *.a *.gcno *.gcda *.gcov gtest* webserver *_test
