.PHONY: default clean distclean run run_c run_pp

MAJOR=0
MINOR=1
LIBS= -ldl -llttng-ust
DLIB=libzipkin-c
DLIBPP=libzipkin-cpp
LIB_DIR=$(shell pwd)
prefix= /usr/local
libdir= $(prefix)/lib
incdir= $(prefix)/include

H_FILES= zipkin_c.h zipkin_trace.h ztracer.hpp

default: $(DLIB).so $(DLIBPP).so test testpp

$(DLIBPP).so: $(DLIBPP).$(MAJOR).so
	ln -sf $< $@

$(DLIBPP).$(MAJOR).so: $(DLIBPP).$(MAJOR).$(MINOR).so
	ln -sf $< $@

$(DLIBPP).$(MAJOR).$(MINOR).so: ztracer.o $(DLIB).so
	gcc -shared -o $@ $< -L. -lzipkin-c

ztracer.o: ztracer.cc ztracer.hpp
	gcc -I. -Wall -fpic -c -o $@ $<

$(DLIB).so: $(DLIB).$(MAJOR).so
	ln -sf $< $@

$(DLIB).$(MAJOR).so: $(DLIB).$(MAJOR).$(MINOR).so
	ln -sf $< $@

$(DLIB).$(MAJOR).$(MINOR).so: zipkin_c.o tp.o
	gcc -shared -o $@ $^ $(LIBS)

zipkin_c.o: zipkin_c.c zipkin_c.h zipkin_trace.h
	gcc -I. -Wall -fpic -c -o $@ $<

tp.o: tp.c zipkin_trace.h
	gcc -I. -fpic -c -o $@ $<

test: test.c $(DLIB).so
	gcc test.c -o test -L. -lzipkin-c

testpp: test.cc $(DLIBPP).so
	LD_LIBRARY_PATH=$(LIB_DIR) g++ $< -o testpp -I. -L. -lboost_thread -lzipkin-cpp

testppp: test_p.cc $(DLIBPP).so
	LD_LIBRARY_PATH=$(LIB_DIR) g++ $< -o testppp -I. -L. -lboost_thread -lzipkin-cpp

run_c:
	LD_LIBRARY_PATH=$(LIB_DIR) ./test

run_pp:
	LD_LIBRARY_PATH=$(LIB_DIR) ./testpp

run_ppp:
	LD_LIBRARY_PATH=$(LIB_DIR) ./testppp

run: run_c run_pp

install:
	install -m 644 $(DLIBPP).$(MAJOR).$(MINOR).so $(DESTDIR)/$(libdir)
	cp -P $(DLIBPP).$(MAJOR).so $(DESTDIR)/$(libdir)
	cp -P $(DLIBPP).so $(DESTDIR)/$(libdir)
	install -m 644  $(DLIB).$(MAJOR).$(MINOR).so $(DESTDIR)/$(libdir)
	cp -P $(DLIB).$(MAJOR).so $(DESTDIR)/$(libdir)
	cp -P $(DLIB).so $(DESTDIR)/$(libdir)
	install -m 644 $(H_FILES) $(DESTDIR)/$(incdir)

clean:
	rm -f *.o *.so test testpp testppp

distclean: clean
	rm -f socket
