# Point to our macro directory and pick up user flags from the environment
ACLOCAL_AMFLAGS  = -I m4 ${ACLOCAL_FLAGS}

# -- bin/ ----------------------------
bin_PROGRAMS = \
	bin/libcangjie_dbbuilder \
	$(NULL)
bin_libcangjie_dbbuilder_SOURCES = \
	tools/dbbuilder.cpp \
	$(NULL)
bin_libcangjie_dbbuilder_LDADD = $(DBCXX_LIBS)

# -- src/ ----------------------------
lib_LTLIBRARIES = libcangjie.la
libcangjie_la_SOURCES = \
	src/cangjie.cpp \
	$(NULL)
libcangjie_la_CXXFLAGS = -I./src
libcangjie_la_LDFLAGS = -version-info $(CANGJIE_SO_VERSION)
libcangjie_la_LIBADD = $(DBCXX_LIBS)

libcangjie_la_include_HEADERS = \
	src/cangjie.h \
	$(NULL)
libcangjie_la_includedir = $(includedir)/cangjie

src/cangjieconfig.h: src/cangjieconfig.h.in Makefile
	$(MKDIR_P) src
	sed -e 's&@PKGDATADIR@&$(pkgdatadir)&g' $< > $@

# -- data/ ---------------------------
pkgconfig_DATA = \
	data/cangjie.pc \
	$(NULL)
pkgconfigdir = $(libdir)/pkgconfig

data/cj3-cc.mb: bin/libcangjie_dbbuilder tables/cj3-20902.txt
	$(MKDIR_P) data
	$^ $@ >/dev/null

data/cj3-cjk.mb: bin/libcangjie_dbbuilder tables/cj3-70000.txt
	$(MKDIR_P) data
	$^ $@ >/dev/null

data/cj3-sc.mb: bin/libcangjie_dbbuilder tables/cj3-6763.txt
	$(MKDIR_P) data
	$^ $@ >/dev/null

data/cj3-tc.mb: bin/libcangjie_dbbuilder tables/cj3-13053.txt
	$(MKDIR_P) data
	$^ $@ >/dev/null

data/cj5-cc.mb: bin/libcangjie_dbbuilder tables/cj5-20902.txt
	$(MKDIR_P) data
	$^ $@ >/dev/null

data/cj5-cjk.mb: bin/libcangjie_dbbuilder tables/cj5-70000.txt
	$(MKDIR_P) data
	$^ $@ >/dev/null

data/cj5-sc.mb: bin/libcangjie_dbbuilder tables/cj5-8300.txt
	$(MKDIR_P) data
	$^ $@ >/dev/null

data/cj5-tc.mb: bin/libcangjie_dbbuilder tables/cj5-13053.txt
	$(MKDIR_P) data
	$^ $@ >/dev/null

libcangjie_data_DATA = \
	data/cj3-cc.mb \
	data/cj3-cjk.mb \
	data/cj3-sc.mb \
	data/cj3-tc.mb \
	data/cj5-cc.mb \
	data/cj5-cjk.mb \
	data/cj5-sc.mb \
	data/cj5-tc.mb \
	$(NULL)
libcangjie_datadir = $(pkgdatadir)

# -- Common --------------------------
BUILT_SOURCES = \
	src/cangjieconfig.h \
	$(NULL)

CLEANFILES = \
	__db.* \
	data/*.mb \
	data/*.pc \
	src/cangjieconfig.h \
	$(NULL)

EXTRA_DIST = \
	autogen.sh \
	README.md \
	src/cangjieconfig.h.in \
	tables/cj3-13053.txt \
	tables/cj3-20902.txt \
	tables/cj3-6763.txt \
	tables/cj3-70000.txt \
	tables/cj5-13053.txt \
	tables/cj5-20902.txt \
	tables/cj5-70000.txt \
	tables/cj5-8300.txt \
	tables/LICENSE \
	tests/testdbs.pl \
	$(NULL)

TESTS = \
	tests/testdbs.pl \
	$(NULL)
