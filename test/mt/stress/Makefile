
include ../../../MakeVars

CXXFLAGS := -I. -I../../../include $(CXXFLAGS) #$(EHFLAGS)
LDFLAGS  := -L../../../libs $(LDFLAGS) 
LDLIBS    = -lmicocoss$(VERSION) -lmico$(VERSION) $(CONFLIBS)

all .NOTPARALLEL: .depend client server proxy

client:	bench.o client.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(POSTLD) $@                                                            

server:	bench.o server.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(POSTLD) $@                                                            

proxy:	bench.o proxy.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(POSTLD) $@                                                            

bench.cc bench.h : bench.idl
	$(IDL) bench.idl                                            

clean:
	$(RM) -f *.o core server client proxy bench.h bench.cc ref server.log .depend *~

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend:
	echo "# module dependencies" > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
