
include ../../MakeVars

DIRS = policy initref cmd-line

.PHONY: all $(DIRS)

all: $(DIRS)

$(DIRS):
	$(MAKE) --directory $@

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done

check:
	for i in $(DIRS); do $(MAKE) -C $$i check || exit 1; done

.depend:
	for i in $(DIRS); do $(MAKE) -C $$i .depend || exit 1; done

