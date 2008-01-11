
include ../../MakeVars

DIRS = request-timeout request-timeout-with-manager \
	request-timeout-with-policy-manager \
	request-timeout-with-policy-current \
	connection-timeout connection-timeout-with-policy-manager \
	connection-timeout-with-policy-current

all:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done

check:
	for i in $(DIRS); do $(MAKE) -C $$i check || exit 1; done
