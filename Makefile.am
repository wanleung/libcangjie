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

# -- data/ ---------------------------
libcangjie_data_DATA = \
	data/cj3/cj3-cc.mb \
	data/cj3/cj3-cjk.mb \
	data/cj3/cj3-sc.mb \
	data/cj3/cj3-tc.mb \
	data/cj5/cj5-cc.mb \
	data/cj5/cj5-cjk.mb \
	data/cj5/cj5-sc.mb \
	data/cj5/cj5-tc.mb \
	$(NULL)
libcangjie_datadir = $(pkgdatadir)

# -- Common --------------------------
CLEANFILES = \
	$(NULL)

EXTRA_DIST = \
	autogen.sh \
	data/cj3/cj3-cc.mb \
	data/cj3/cj3-cjk.mb \
	data/cj3/cj3-sc.mb \
	data/cj3/cj3-tc.mb \
	data/cj5/cj5-cc.mb \
	data/cj5/cj5-cjk.mb \
	data/cj5/cj5-sc.mb \
	data/cj5/cj5-tc.mb \
	README.md \
	tables/cj3-13053.txt \
	tables/cj3-20902.txt \
	tables/cj3-6763.txt \
	tables/cj3-70000.txt \
	tables/cj5-13053.txt \
	tables/cj5-20902.txt \
	tables/cj5-70000.txt \
	tables/cj5-8300.txt \
	tables/LICENSE \
	tools/test.pl \
	$(NULL)
