CC=clang
CXX=clang++
LINK=clang++

CFLAGS=-g -I../ -fPIC
CXXFLAGS=-g -std=c++14 -I../ -fPIC
LDFLAGS=-lrt -lpthread -std=c++14 -fPIC

aplib.so:
	$(CXX) aplib.cpp $(CXXFLAGS) -c
	$(LINK) aplib.o -shared -o aplib.so $(LDFLAGS)

clean:
	rm -f aplib.so aplib.o test

test: aplib.so
	$(CXX) test.cpp $(CXXFLAGS) -c
	$(CXX) test.o aplib.o $(LDFLAGS) -o test

indent:
	clang-format -i -style=file `find $d -name '*.cpp' -or -name "*.h" -or -name "*.c" -or -name "*.cc"`

.PHONY: aplib.so
