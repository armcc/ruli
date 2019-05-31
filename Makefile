#
# $Id: Makefile,v 1.9 2004/06/24 05:02:24 evertonm Exp $
#
# This is a Debian-friendly top-level Makefile.
#

.PHONY: default
default:
	$(MAKE) -C src   
	$(MAKE) -C sample 
	$(MAKE) -C ti_sample

.PHONY: clean
clean:
	$(MAKE) -C src    clean
	$(MAKE) -C sample clean
	$(MAKE) -C ti_sample clean

.PHONY: install
install:
	$(MAKE) -C src    install
	$(MAKE) -C sample install
	$(MAKE) -C ti_sample install

.PHONY: dpkg
dpkg:
	dpkg-buildpackage -rfakeroot

.PHONY: dpkg-clean
dpkg-clean:
	fakeroot debian/rules clean

.PHONY: release-clean
release-clean:
	rm -rf debian install doc/rfc ruli-mta test tube \
		`find . -name CVS -o -name "*~"`

