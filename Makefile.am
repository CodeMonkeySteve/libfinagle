AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = Finagle test

include aminclude.am

EXTRA_DIST = $(DX_CONFIG) doc/html
doc/html: $(DIST_SOURCES)
	$(MAKE) doxygen-doc


SPEC_FILE = $(PACKAGE).spec
EXTRA_DIST += $(SPEC_FILE)

RPM_PATH_FMT = $(shell rpm --eval '%{_rpmdir}/%{_build_name_fmt}')
RPM_PATHS = $(shell rpm -q --qf "$(RPM_PATH_FMT) " --specfile $(SPEC_FILE))

pkgs: $(RPM_PATHS)

$(RPM_PATHS): $(SPEC_FILE).in $(DIST_COMMON) $(DIST_SOURCES)
	$(MAKE) dist-bzip2
	rpmbuild -tb $(distdir).tar.bz2	

