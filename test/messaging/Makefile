
include ../../MakeVars

DIRS = timeout timeout-with-manager

all:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done

check:
	for i in $(DIRS); do $(MAKE) -C $$i check || exit 1; done
