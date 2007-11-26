AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = sigslot Finagle

# Add pkg-config file
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc
EXTRA_DIST = @PACKAGE@.pc.in

# Build RPMS
EXTRA_DIST += @PACKAGE@.spec @PACKAGE@.spec.in
RPM_PATH_FMT = $(shell rpm --eval '%{_rpmdir}/%{_build_name_fmt}')
SRPM_PATH_FMT = $(subst %{ARCH},src,$(shell rpm --eval '%{_srcrpmdir}/%{_build_name_fmt}'))
RPM_PATHS = $(shell rpm -q --qf "$(RPM_PATH_FMT) " --specfile @PACKAGE@.spec)
SRPM_PATH = $(shell rpm -q --qf "$(SRPM_PATH_FMT)\n" --specfile @PACKAGE@.spec | head -n 1)

pkgs: $(SRPM_PATH) $(RPM_PATHS)

SRPMDIR ?= $(RPMDIR)/SRPMS
pkgs-install: pkgs
	@mkdir -p $(RPMDIR) $(SRPMDIR)
	@cp $(RPM_PATHS) $(RPMDIR)
	@cp $(SRPM_PATHS) $(SRPMDIR)

$(SRPM_PATH):
	$(MAKE) dist-bzip2
	rpmbuild -ta $(distdir).tar.bz2
