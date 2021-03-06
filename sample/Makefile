
#
# $Id: Makefile,v 1.20 2005/09/01 11:16:20 evertonm Exp $
#

#
# OOP_INCLUDE_DIR points the location of
# Liboop headers in your system
#
# OOP_LIB_DIR points the location of
# Liboop libraries in your system
#
# Debian packaging likes this: OOP_BASE_DIR = /usr
#
OOP_BASE_DIR    = /usr/local/oop
OOP_INCLUDE_DIR = $(OOP_BASE_DIR)/include
OOP_LIB_DIR     = $(OOP_BASE_DIR)/lib

#
# INSTALL_BASE_DIR indicates where RULI
# is to be installed in your system
#
# Debian packaging likes this: INSTALL_BASE_DIR = $(DESTDIR)/usr
#
INSTALL_BASE_DIR    = ../install/ruli
INSTALL_INCLUDE_DIR = $(INSTALL_BASE_DIR)/include
INSTALL_LIB_DIR     = $(INSTALL_BASE_DIR)/lib
INSTALL_BIN_DIR     = $(INSTALL_BASE_DIR)/bin
INSTALL_MAN_DIR     = $(INSTALL_BASE_DIR)/share/man
INSTALL_MAN1_DIR    = $(INSTALL_MAN_DIR)/man1

PLATFORM=$(shell uname)

ifeq ($(PLATFORM),SunOS)
SOLARIS_LIBS = -lresolv -lnsl -lsocket
endif

CC      = gcc
CFLAGS  = -O2 -pipe -Wall -g -I../src -I$(OOP_INCLUDE_DIR)
LDFLAGS = -L../src -L$(OOP_LIB_DIR) $(SOLARIS_LDFLAGS)
UNRULI_LINK = $(CC) $(ARCH) $(LDFLAGS) -o $@ $^ $(SOLARIS_LIBS)
LINK = $(UNRULI_LINK) -lruli -loop

TARGETS = srvsearch sync_srvsearch smtpsearch sync_smtpsearch \
	sync_httpsearch httpsearch ruli-getaddrinfo getaddrinfo
UNRULI_SHARED_OBJS = parse_options.o
SHARED_OBJS = $(UNRULI_SHARED_OBJS) stdout_srv_list.o

.PHONY: default
default: $(TARGETS)

.PHONY: clean
clean:
	rm -f *~ .*~ *.o $(TARGETS)

.PHONY: build
build: clean default

.PHONY: strip
strip: $(TARGETS)
	strip $(TARGETS)

.PHONY: install
install: $(TARGETS)
	mkdir -p $(INSTALL_BIN_DIR) $(INSTALL_MAN1_DIR)
	cp $(TARGETS) $(INSTALL_BIN_DIR)
	cp ../doc/man/*.1 $(INSTALL_MAN1_DIR)
	cd $(INSTALL_MAN_DIR)/man1 && ln -s srvsearch.1 smtpsearch.1
	cd $(INSTALL_MAN_DIR)/man1 && ln -s srvsearch.1 httpsearch.1
	cd $(INSTALL_MAN_DIR)/man1 && ln -s srvsearch.1 sync_srvsearch.1
	cd $(INSTALL_MAN_DIR)/man1 && ln -s srvsearch.1 sync_smtpsearch.1
	cd $(INSTALL_MAN_DIR)/man1 && ln -s srvsearch.1 sync_httpsearch.1
	cd $(INSTALL_MAN_DIR)/man1 && ln -s srvsearch.1 ruli-getaddrinfo.1
	cd $(INSTALL_MAN_DIR)/man1 && ln -s srvsearch.1 getaddrinfo.1

%.o: %.c
	$(CC) $(ARCH) $(CPPFLAGS) $(CFLAGS) -c $<

srvsearch: srvsearch.o $(SHARED_OBJS)
	$(LINK)

sync_srvsearch: sync_srvsearch.o $(SHARED_OBJS)
	$(LINK)

smtpsearch: smtpsearch.o $(SHARED_OBJS)
	$(LINK)

sync_smtpsearch: sync_smtpsearch.o $(SHARED_OBJS)
	$(LINK)

httpsearch: httpsearch.o $(SHARED_OBJS)
	$(LINK)

sync_httpsearch: sync_httpsearch.o $(SHARED_OBJS)
	$(LINK)

ruli-getaddrinfo: ruli-getaddrinfo.o addrprint.o $(UNRULI_SHARED_OBJS)
	$(LINK)

getaddrinfo: getaddrinfo.o run_getaddrinfo.o addrprint.o $(UNRULI_SHARED_OBJS)
	$(UNRULI_LINK) -ldl
