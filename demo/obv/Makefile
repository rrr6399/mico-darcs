
include ../MakeVars

DIRS=abstract custom date tree tricky

.PHONY: all $(DIRS)

all: $(DIRS)

$(DIRS):
	$(MAKE) --directory $@

run:
	for i in $(DIRS); do $(MAKE) -C $$i run || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done
	rm -f *.o core *~ .depend

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/obv
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/obv
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples/obv
	for i in $(DIRS); do $(MAKE) -C $$i install || exit 1; done
