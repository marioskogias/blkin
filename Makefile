.PHONY: default clean distclean

prefix= /usr/local
libdir= $(prefix)/lib
incdir= $(prefix)/include

DFLAGS= -ldl -llttng-ust  
DLIB=libblkin.so
LIB_DIR=$(shell pwd)

H_FILES= blkin.h zipkin_trace.h

default: $(DLIB)

$(DLIB): blkin.o tp.o
	gcc -shared -o $@ $^ $(DFLAGS)

blkin.o: blkin.c blkin.h zipkin_trace.h
	gcc -I. -Wall -fpic -c -o $@ $<

tp.o: tp.c zipkin_trace.h
	gcc -I. -fpic -c -o $@ $<

test: test.c $(DLIB)
	gcc -I. test.c -o test -L. -lblkin

run:
	LD_PRELOAD=/usr/local/lib/liblttng-ust-fork.so LD_LIBRARY_PATH=$(LIB_DIR) ./test

install:
	install $(DLIB) $(libdir)
	install $(H_FILES) $(incdir) 
	ldconfig
clean: 
	rm -f *.o 

distclean:
	make clean
	rm -f $(DLIB) test
