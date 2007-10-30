AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = Finagle test

EXTRA_DIST = Doxyfile Doxyfile.in doc/html $(srcdir)/doc/*
doc/html: Doxyfile $(DIST_SOURCES)
	@rm -rf doc/html
	@doxygen Doxyfile >/dev/null

# Add pkg-config file
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc 
EXTRA_DIST += @PACKAGE@.pc.in

# Build RPMS
EXTRA_DIST += @PACKAGE@.spec @PACKAGE@.spec.in
RPM_PATH_FMT = $(shell rpm --eval '%{_rpmdir}/%{_build_name_fmt}')
RPM_PATHS = $(shell rpm -q --qf "$(RPM_PATH_FMT) " --specfile @PACKAGE@.spec)

pkgs: $(RPM_PATHS)

pkgs-install: pkgs
	cp $(RPM_PATHS) $(PKGDIR)

$(RPM_PATHS): @PACKAGE@.spec.in $(DIST_SOURCES)
	$(MAKE) dist-bzip2
	rpmbuild -tb $(distdir).tar.bz2	

