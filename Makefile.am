# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = Finagle test

SPEC_FILE=$(PACKAGE).spec

EXTRA_DIST = libFinagle.pc $(SPEC_FILE)


RPM_DIR=`rpm --eval '%{_rpmdir}'`
RPM_FILES=`rpm -q --qf "`rpm --eval '%{_build_name_fmt}\n'`" --specfile $(SPEC_FILE)
RPM_PATHS=$RPM_DIR/$RPM_FILE

rpm: $(RPM_PATHS)

$(RPM_PATHS): $(SPEC_FILE) dist-bzip2
	rpmbuild -tb $(distdir).tar.bz2	

