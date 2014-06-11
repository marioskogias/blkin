.PHONY: default clean distclean

DFLAGS= -ldl -llttng-ust  
DLIB=libzipkin-c.so
LIB_DIR=$(shell pwd)

default: $(DLIB)

$(DLIB): zipkin_c.o tp.o
	gcc -shared -o $@ $^ $(DFLAGS)

zipkin_c.o: zipkin_c.c zipkin_c.h zipkin_trace.h
	gcc -I. -Wall -fpic -c -o $@ $<

tp.o: tp.c zipkin_trace.h
	gcc -I. -fpic -c -o $@ $<

test: test.c $(DLIB)
	gcc test.c -o test -L. -lzipkin-c

run:
	LD_LIBRARY_PATH=$(LIB_DIR) ./test

clean: 
	rm -f *.o 

distclean:
	make clean
	rm -f $(DLIB) test socket
