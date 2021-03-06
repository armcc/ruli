#
# $Id: Makefile,v 1.25 2005/12/21 10:08:31 evertonm Exp $
#

#
# OOP_INCLUDE_DIR points the location of
# Liboop headers in your system
#
# OOP_LIB_DIR points the location of
# Liboop libraries in your system
#
OOP_BASE_DIR    = /usr/local/oop
OOP_INCLUDE_DIR = $(OOP_BASE_DIR)/include
OOP_LIB_DIR     = $(OOP_BASE_DIR)/lib

#
# INSTALL_BASE_DIR indicates where RULI
# is to be installed in your system
#
INSTALL_BASE_DIR    = ../install/ruli
INSTALL_INCLUDE_DIR = $(INSTALL_BASE_DIR)/include
INSTALL_LIB_DIR     = $(INSTALL_BASE_DIR)/lib
INSTALL_BIN_DIR     = $(INSTALL_BASE_DIR)/bin

WARN = -pedantic -ansi -Wshadow -Wpointer-arith -Wcast-qual \
        -Wcast-align -Wwrite-strings -Wredundant-decls \
	-Wall

# gcc-specific options (not valid for g++)
ifeq ($(CC),gcc)
WARN += -Wnested-externs
WARN += -Wmissing-prototypes
WARN += -Wstrict-prototypes
endif

CC      = gcc
CFLAGS  = $(WARN) -O2 -pipe -g -I../src -I$(OOP_INCLUDE_DIR)

LDFLAGS = -L../src -L$(OOP_LIB_DIR) -lruli -loop

TARGETS = list resolve hostsolver srvsolver syncsolver addrsolver \
	srvsolver2 addrsolver2 addrsolver3 ruli-host rand cycle_res \
        cycle_res2 ipv6 rfc3484 in6_addr

LINK = $(CC) $(ARCH) $(LDFLAGS) -o $@ $^

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
	[ -d $(INSTALL_BIN_DIR) ] || mkdir -p $(INSTALL_BIN_DIR) || exit
	cp $(TARGETS) $(INSTALL_BIN_DIR)

%.o: %.c
	$(CC) $(ARCH) $(CPPFLAGS) $(CFLAGS) -c $<

list: list.o
	$(LINK)

rand: rand.o
	$(LINK)

resolve: resolve.o trivial_conf_handler.o
	$(LINK)

hostsolver: hostsolver.o stdin_domains.o trivial_conf_handler.o
	$(LINK)

srvsolver: srvsolver.o stdin_domains.o stdout_srv_list.o trivial_conf_handler.o
	$(LINK)

srvsolver2: srvsolver2.o stdout_srv_list.o trivial_conf_handler.o
	$(LINK)

syncsolver: syncsolver.o stdout_srv_list.o
	$(LINK)

addrsolver: addrsolver.o trivial_conf_handler.o
	$(LINK)

addrsolver2: addrsolver2.o trivial_conf_handler.o
	$(LINK)

addrsolver3: addrsolver3.o trivial_conf_handler.o
	$(LINK)

ruli-host: ruli-host.o trivial_conf_handler.o
	$(LINK)

cycle_res: cycle_res.o trivial_conf_handler.o
	$(LINK)

cycle_res2: cycle_res2.o trivial_conf_handler.o
	$(LINK)

ipv6: ipv6.o
	$(LINK)

rfc3484: rfc3484.o
	$(LINK)

in6_addr: in6_addr.c
	$(CC) $(CFLAGS) -o $@ $<

