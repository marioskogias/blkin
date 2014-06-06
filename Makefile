.PHONY: default clean distclean

DFLAGS= -ldl -llttng-ust  
DLIB=libzipkin-c.so

default: $(DLIB)

$(DLIB): zipkin_c.o tp.o
	gcc -shared -o $@ $< $(DFLAGS)

zipkin_c.o: zipkin_c.c zipkin_c.h zipkin_trace.h
	gcc -I. -Wall -fpic -c -o $@ $<

tp.o: tp.c zipkin_trace.h
	gcc -I. -fpic -c -o $@ $<

clean: 
	rm -f *.o

distclean:
	make clean
	rm -f $(DLIB)
