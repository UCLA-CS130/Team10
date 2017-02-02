all: clean release

CPPFLAGS+=-std=c++11 -Wall
CPPFLAGS+=-g

CPPFLAGS+=-pthread
LDFLAGS+=-lboost_system

GTEST_DIR=googletest/googletest
TESTS=config_parser_test
TEST_CLASSES=config_parser.cpp
CLASSES=config_parser.o connection.o connection_manager.o main.o mime_types.o reply.o request_handler.o request_parser.o server.o

.PHONY: clean test release

debug: webserver
debug: test
	for test in $(TEST_CLASSES); do gcov -r $$test; done

release: CPPFLAGS+=-O3 -pedantic
release: webserver

libgtest.a:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

%_test: %_test.cpp libgtest.a
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread $(TEST_CLASSES) $(TESTS:=.cpp) ${GTEST_DIR}/src/gtest_main.cc libgtest.a -fprofile-arcs -ftest-coverage -o $@

#%.o: $(CLASSES)
#	$(CXX) -Werror $(CPPFLAGS) $^ -c

test: $(TESTS)
	for test in $(TESTS); do ./$$test; done

webserver: $(CLASSES)
	$(CXX) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o *.a *.gcno *.gcda *.gcov gtest* webserver config_parser_test
