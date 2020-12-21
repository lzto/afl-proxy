SUBDIRS := proxy send aplib

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
.PHONY: all $(SUBDIRS)

clean:
	rm -rf proxy/build send/build

indent:
	clang-format -i -style=file `find $d -name '*.cpp' -or -name "*.h" -or -name "*.c" -or -name "*.cc"`

