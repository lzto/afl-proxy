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
	rm -f aplib.so aplib.o

indent:
	clang-format -i -style=file `find $d -name '*.cpp' -or -name "*.h" -or -name "*.c" -or -name "*.cc"`

