# Copyright 2015 Marios Kogias <marioskogias@gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or
# without modification, are permitted provided that the following
# conditions are met:
#
#   1. Redistributions of source code must retain the above
#      copyright notice, this list of conditions and the following
#      disclaimer.
#   2. Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials
#      provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

.PHONY: default clean distclean run run_c run_pp

MAJOR=0
MINOR=1
LIBS= -ldl -llttng-ust
DLIB=libzipkin-c
DLIBPP=libzipkin-cpp
DLIBFRONT=libblkin-front

LIB_DIR=$(shell pwd)
prefix= /usr/local
libdir= $(prefix)/lib
incdir= $(prefix)/include

H_FILES= zipkin_c.h zipkin_trace.h ztracer.hpp blkin-front.h

default: $(DLIB).so $(DLIBPP).so $(DLIBFRONT).so test testpp testppp

$(DLIBFRONT).so: $(DLIBFRONT).$(MAJOR).so
	ln -sf $< $@

$(DLIBFRONT).$(MAJOR).so: $(DLIBFRONT).$(MAJOR).$(MINOR).so
	ln -sf $< $@

$(DLIBFRONT).$(MAJOR).$(MINOR).so: blkin-front.o
	gcc -shared -g -o $@ $< -ldl

blkin-front.o: blkin-front.c
	gcc -I. -g -Wall -fpic -c -o $@ $<

$(DLIBPP).so: $(DLIBPP).$(MAJOR).so
	ln -sf $< $@

$(DLIBPP).$(MAJOR).so: $(DLIBPP).$(MAJOR).$(MINOR).so
	ln -sf $< $@

$(DLIBPP).$(MAJOR).$(MINOR).so: ztracer.o $(DLIBFRONT).so
	gcc -shared -g -o $@ $< -L. -lblkin-front

ztracer.o: ztracer.cc ztracer.hpp
	gcc -I. -Wall -fpic -g -c -o $@ $<

$(DLIB).so: $(DLIB).$(MAJOR).so
	ln -sf $< $@

$(DLIB).$(MAJOR).so: $(DLIB).$(MAJOR).$(MINOR).so
	ln -sf $< $@

$(DLIB).$(MAJOR).$(MINOR).so: zipkin_c.o tp.o
	gcc -shared -o $@ $^ $(LIBS)

zipkin_c.o: zipkin_c.c zipkin_c.h zipkin_trace.h
	gcc -I. -Wall -fpic -g -c -o $@ $<

tp.o: tp.c zipkin_trace.h
	gcc -I. -fpic -g -c -o $@ $<

test: test.c $(DLIBFRONT).so
	gcc test.c -o test -g -I. -L. -lblkin-front

testpp: test.cc $(DLIBPP).so
	LD_LIBRARY_PATH=$(LIB_DIR) g++ $< -o testpp -g -I. -L. -lboost_thread -lboost_system -lzipkin-cpp

testppp: test_p.cc $(DLIBPP).so
	LD_LIBRARY_PATH=$(LIB_DIR) g++ $< -o testppp -g -I. -L. -lboost_thread -lboost_system -lzipkin-cpp

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
	install -m 644 $(DLIBFRONT).$(MAJOR).$(MINOR).so $(DESTDIR)/$(libdir)
	cp -P $(DLIBFRONT).$(MAJOR).so $(DESTDIR)/$(libdir)
	cp -P $(DLIBFRONT).so $(DESTDIR)/$(libdir)
	install -m 644 $(H_FILES) $(DESTDIR)/$(incdir)

clean:
	rm -f *.o *.so test testpp testppp

distclean: clean
	rm -f socket
