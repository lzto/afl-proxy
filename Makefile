SUBDIRS := proxy aplib

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  SUBDIRS += send
endif
ifeq ($(UNAME_S),Darwin)
  LDFLAGS += 
endif

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
  done

indent:
	clang-format -i -style=file `find $d -name '*.cpp' -or -name "*.h" -or -name "*.c" -or -name "*.cc" -or -name "*.cpp"`

