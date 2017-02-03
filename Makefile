all:webserver

CPPFLAGS+=-std=c++11 -Wall -Werror -pedantic
CPPFLAGS+=-g -O2

CPPFLAGS+=-pthread
LDFLAGS+=-lboost_system

%.o:	%.cpp	;	$(CXX) $(CPPFLAGS) $^ -c -o $@

webserver:	$(patsubst %.cpp,%.o,$(wildcard *.cpp))	;	$(CXX) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o webserver

int:  ;	 python test.py 
