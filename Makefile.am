# Point to our macro directory and pick up user flags from the environment
ACLOCAL_AMFLAGS  = -I m4 ${ACLOCAL_FLAGS}

# -- bin/ ----------------------------
bin_PROGRAMS = \
	bin/libcangjie_cli \
	bin/libcangjie_dbbuilder \
	bin/wordfreq_dbbuilder \
	$(NULL)

bin_libcangjie_cli_SOURCES = \
	src/test.cpp \
	$(NULL)
bin_libcangjie_cli_LDADD = $(DBCXX_LIBS) libutil.la libcangjie.la

bin_libcangjie_dbbuilder_SOURCES = \
	src/dbbuilder.cpp \
	$(NULL)
bin_libcangjie_dbbuilder_LDADD = $(DBCXX_LIBS) libutil.la

bin_wordfreq_dbbuilder_SOURCES = \
	src/freqbuilder.cpp \
	$(NULL)
bin_wordfreq_dbbuilder_LDADD = $(DBCXX_LIBS)

# -- src/ ----------------------------
lib_LTLIBRARIES = libcangjie.la
libcangjie_la_SOURCES = \
	src/fullchar.cpp \
	src/cangjie.cpp \
	$(NULL)
libcangjie_la_CXXFLAGS = -I./src
libcangjie_la_LDFLAGS = -version-info $(CANGJIE_SO_VERSION)
libcangjie_la_LIBADD = $(DBCXX_LIBS) libutil.la

noinst_LTLIBRARIES = libutil.la
libutil_la_SOURCES = \
	src/util.cpp \
	src/util.h \
	$(NULL)
libutil_la_CXXFLAGS = -I./src

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

data/%.mb: bin/libcangjie_dbbuilder tables/%.txt
	$(MKDIR_P) data
	$^ $@ >/dev/null

data/wordfreq.mb: bin/wordfreq_dbbuilder words/frequency.txt
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
	data/wordfreq.mb \
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
	tables/cj3-cc.txt \
	tables/cj3-cjk.txt \
	tables/cj3-sc.txt \
	tables/cj3-tc.txt \
	tables/cj5-cc.txt \
	tables/cj5-cjk.txt \
	tables/cj5-sc.txt \
	tables/cj5-tc.txt \
	tables/COPYING.tables \
	tables/README.tables \
	tests/testdbs.pl \
	words/README \
	words/countfreq.sh \
	words/getcorpus.cpp \
	words/frequency.txt \
	$(NULL)

TESTS = \
	tests/testdbs.pl \
	$(NULL)
