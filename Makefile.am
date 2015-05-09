# Copyright 2014 Marios Kogias <marioskogias@gmail.com>
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

ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = README.md INSTALL.md blkin.spec blkin.pc

noinst_PROGRAMS = src/tests/testc src/tests/testpp src/tests/testppp
lib_LTLIBRARIES = src/libblkin-front.la src/libzipkin-c.la src/libzipkin-cpp.la
pkginclude_HEADERS = src/zipkin_c.h src/zipkin_trace.h src/ztracer.hpp src/blkin-front.h

pkgconfigdir = $(libdir)/pkgconfig
dist_pkgconfig_DATA = blkin.pc

AM_CPPFLAGS = -Isrc -I$(srcdir)/src

src_tests_testc_SOURCES = src/tests/testc.c
src_tests_testc_LDADD = src/libblkin-front.la

src_tests_testpp_SOURCES = src/tests/test.cc
src_tests_testpp_LDADD = src/libzipkin-cpp.la -lboost_thread -lboost_system

src_tests_testppp_SOURCES = src/tests/test_p.cc
src_tests_testppp_LDADD = src/libzipkin-cpp.la -lboost_thread -lboost_system

src_libblkin_front_la_SOURCES = src/blkin-front.c
src_libblkin_front_la_LIBADD = -ldl -llttng-ust

src_libzipkin_cpp_la_SOURCES = src/ztracer.cc src/ztracer.hpp
src_libzipkin_cpp_la_LIBADD = src/libblkin-front.la

src_libzipkin_c_la_SOURCES = src/zipkin_c.c src/zipkin_c.h src/zipkin_trace.h src/tp.c
src_libzipkin_c_la_LIBADD = -ldl -llttng-ust

dist_doc_DATA = README.md INSTALL.md COPYRIGHT

if WITH_BABEL
dist_bin_SCRIPTS = \
babeltrace-plugins/zipkin/src/babeltrace_zipkin \
babeltrace-plugins/json/src/babeltrace_json

CLEANFILES = $(dist_bin_SCRIPTS)

babeltrace-plugins/zipkin/src/babeltrace_zipkin: $(srcdir)/babeltrace-plugins/zipkin/src/babeltrace_zipkin.py
	cp $(srcdir)/babeltrace-plugins/zipkin/src/babeltrace_zipkin.py $@
	chmod +x $@

babeltrace-plugins/json/src/babeltrace_json: $(srcdir)/babeltrace-plugins/json/src/babeltrace_json.py
	cp $(srcdir)/babeltrace-plugins/json/src/babeltrace_json.py $@
	chmod +x $@

zipkin_logicdir = $(pythondir)/zipkin_logic
zipkin_logic_PYTHON = \
babeltrace-plugins/zipkin/src/zipkin_logic/formatters.py \
babeltrace-plugins/zipkin/src/zipkin_logic/trace.py \
babeltrace-plugins/zipkin/src/zipkin_logic/__init__.py \
babeltrace-plugins/zipkin/src/zipkin_logic/zipkin_client.py \
babeltrace-plugins/zipkin/src/zipkin_logic/ttypes.py

scribe_clientdir = $(pythondir)/scribe_client
scribe_client_PYTHON = \
babeltrace-plugins/scribe_client/scribe_client.py \
babeltrace-plugins/scribe_client/__init__.py

noinst_SCRIPTS = \
debian/blkin-babeltrace.dirs \
debian/blkin-babeltrace.install \
debian/postinst \
debian/postrm

py_subst = sed -e 's![@]PYTHONDIR[@]!$(pythondir)!g'

debian/blkin-babeltrace.dirs: $(srcdir)/debian/blkin-babeltrace.dirs.in
	$(py_subst) $(srcdir)/debian/blkin-babeltrace.dirs.in > $@
debian/blkin-babeltrace.install: $(srcdir)/debian/blkin-babeltrace.install.in
	$(py_subst) $(srcdir)/debian/blkin-babeltrace.install.in > $@
debian/postinst: $(srcdir)/debian/postinst.in
	$(py_subst) $(srcdir)/debian/postinst.in > $@
debian/postrm: $(srcdir)/debian/postrm.in
	$(py_subst) $(srcdir)/debian/postrm.in > $@
endif # WITH_BABEL
