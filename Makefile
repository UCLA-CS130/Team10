all:server

CPPFLAGS+=-std=c++11 -Wall -pedantic
CPPFLAGS+=-g -O2

CPPFLAGS+=-pthread
LDFLAGS+=-lboost_system

%.o:	%.cpp	;	$(CXX) $(CPPFLAGS) $^ -c -o $@

server:	$(patsubst %.cpp,%.o,$(wildcard *.cpp))	;	$(CXX) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)
