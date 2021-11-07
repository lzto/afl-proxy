#
# makefile for aplib 
# 2019-2021 Tong Zhang<ztong0001@gmail.com>
#
CC=clang
CXX=clang++
LINK=clang++

UNAME_S := $(shell uname -s)

CFLAGS=-g -I../ -fPIC
CXXFLAGS=-g -std=c++14 -I../ -fPIC

LDFLAGS=-lpthread -std=c++14 -fPIC

ifeq ($(UNAME_S),Linux)
  LDFLAGS += -lrt
endif
ifeq ($(UNAME_S),Darwin)
  LDFLAGS += 
endif


aplib.so:
	$(CXX) hw_model.cpp $(CXXFLAGS) -c
	$(CXX) aplib.cpp $(CXXFLAGS) -c
	$(LINK) aplib.o hw_model.o -shared -o aplib.so $(LDFLAGS)

clean:
	rm -f aplib.so aplib.o hw_model.o test

test: aplib.so
	$(CXX) test.cpp $(CXXFLAGS) -c
	$(CXX) hw_model.cpp $(CXXFLAGS) -c
	$(CXX) test.o aplib.o hw_model.o $(LDFLAGS) -o test

indent:
	clang-format -i -style=file `find $d -name '*.cpp' -or -name "*.h" -or -name "*.c" -or -name "*.cc"`

.PHONY: aplib.so
