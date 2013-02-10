# Point to our macro directory and pick up user flags from the environment
ACLOCAL_AMFLAGS  = -I m4 ${ACLOCAL_FLAGS}

# -- bin/ ----------------------------
bin_PROGRAMS = \
	bin/libcangjie_cli \
	bin/libcangjie_dbbuilder \
	bin/libcangjie_char_type_builder \
	bin/wordfreq_dbbuilder \
	$(NULL)

bin_libcangjie_cli_SOURCES = \
	src/cli.cpp \
	$(NULL)
bin_libcangjie_cli_LDADD = $(DBCXX_LIBS) libutil.la libcangjie.la

bin_libcangjie_dbbuilder_SOURCES = \
	src/dbbuilder.cpp \
	$(NULL)
bin_libcangjie_dbbuilder_LDADD = $(DBCXX_LIBS) libutil.la

bin_libcangjie_char_type_builder_SOURCES = \
	src/char_type_builder.cpp \
	$(NULL)
bin_libcangjie_char_type_builder_LDADD = $(DBCXX_LIBS) libutil.la

bin_wordfreq_dbbuilder_SOURCES = \
	src/freqbuilder.cpp \
	$(NULL)
bin_wordfreq_dbbuilder_LDADD = $(DBCXX_LIBS)

# -- src/ ----------------------------
lib_LTLIBRARIES = libcangjie.la
libcangjie_la_SOURCES = \
	src/fullchar.cpp \
	src/chchar.cpp \
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

data/wordfreq.mb: bin/wordfreq_dbbuilder freq-data/frequency.txt
	$(MKDIR_P) data
	$^ $@ >/dev/null

data/tc.mb: bin/libcangjie_char_type_builder filter/tc.txt
	$(MKDIR_P) data
	$^ $@ >/dev/null

data/sc.mb: bin/libcangjie_char_type_builder filter/sc.txt
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
	data/tc.mb \
	data/sc.mb \
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
	freq-data/README \
	freq-data/countfreq.sh \
	freq-data/getcorpus.cpp \
	freq-data/frequency.txt \
	filter/tc.txt \
	filter/sc.txt \
	$(NULL)

TESTS = \
	tests/testdbs.pl \
	$(NULL)
