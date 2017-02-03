#!/bin/bash

GTEST_DIR=googletest/googletest
GMOCK_DIR=googletest/googlemock
case `uname` in
  Linux)
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc

    g++ -std=c++0x -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -I${GMOCK_DIR} -I${GTEST_DIR} -pthread -c ${GMOCK_DIR}/src/gmock-all.cc
    ar -rv libgtest.a gtest-all.o
    ar -rv libgmock.a gmock-all.o

    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread config_parser_test.cpp ../config_parser.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test

    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread mime_types_test.cpp ../mime_types.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o mime_types_test

    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread connection_test.cpp ../connection.cpp ../reply.cpp ../request_handler.cpp ../request_parser.cpp ../mime_types.cpp ../connection_manager.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o connection_test -lboost_system

    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread connection_manager_test.cpp ../connection.cpp ../reply.cpp ../request_handler.cpp ../request_parser.cpp ../mime_types.cpp ../connection_manager.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o connection_manager_test -lboost_system



    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread request_parser_test.cpp ../request_parser.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_parser_test -lboost_system

  g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread request_handler_test.cpp ../reply.cpp ../request_handler.cpp ../mime_types.cpp  ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handler_test -lboost_system

  g++ -std=c++0x -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -pthread server_test.cpp ../server.cpp ../connection.cpp ../request_parser.cpp ../mime_types.cpp ../reply.cpp ../request_handler.cpp ../connection_manager.cpp  ${GMOCK_DIR}/src/gmock_main.cc libgtest.a libgmock.a -o server_test -lboost_system



  g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread reply_test.cpp ../reply.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o reply_test -lboost_system
 
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread request_test.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_test -lboost_system;;



  Darwin)
    clang++ -std=c++11 -stdlib=libc++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
    ar -rv libgtest.a gtest-all.o
    clang++ -std=c++11 -stdlib=libc++ -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test ;;
  *)
    echo "Unknown operating system." ;;
esac
