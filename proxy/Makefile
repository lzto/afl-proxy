CC=clang
CXX=clang++
LINK=clang++

CFLAGS=-g -I../
CXXFLAGS=-g -std=c++14 -I../
LDFLAGS= -lpthread -std=c++14
TARGET_EXEC=ap

ifeq ($(UNAME_S),Linux)
  LDFLAGS += -lrt
endif
ifeq ($(UNAME_S),Darwin)
  LDFLAGS += 
endif

JUNK=dry_run_stamp*

-include ../Makefile.defaults

