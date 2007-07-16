AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = Finagle test

# Build documentation
include aminclude.am

EXTRA_DIST = $(DX_CONFIG) doc/html
doc/html: $(DIST_SOURCES)
	$(MAKE) doxygen-doc


# Add pkg-config file
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc

# Build RPMS
EXTRA_DIST += @PACKAGE@.spec 
RPM_PATH_FMT = $(shell rpm --eval '%{_rpmdir}/%{_build_name_fmt}')
RPM_PATHS = $(shell rpm -q --qf "$(RPM_PATH_FMT) " --specfile @PACKAGE@.spec)

pkgs: $(RPM_PATHS)

$(RPM_PATHS): @PACKAGE@.spec.in $(DIST_COMMON) $(DIST_SOURCES)
	$(MAKE) dist-bzip2
	rpmbuild -tb $(distdir).tar.bz2	

