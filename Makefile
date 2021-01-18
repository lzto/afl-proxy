SUBDIRS := proxy send aplib

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
  done

indent:
	clang-format -i -style=file `find $d -name '*.cpp' -or -name "*.h" -or -name "*.c" -or -name "*.cc"`

