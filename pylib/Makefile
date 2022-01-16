CC=clang
CXX=clang++
LINK=clang++

CFLAGS=-g -I../ -fPIC
CXXFLAGS=-g -std=c++14 -I../ -fPIC
LDFLAGS= -lpthread -std=c++14 -shared -fPIC
TARGET_EXEC=pyaplib.so

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
  LDFLAGS += -lrt
endif
ifeq ($(UNAME_S),Darwin)
  LDFLAGS += 
endif

-include ../Makefile.defaults

