#
# $Id: Makefile,v 1.6 2004/10/26 21:13:15 evertonm Exp $
#

INCLUDES = -I../src -I/usr/local/ruli/include
LIBS = -L../src -L/usr/local/ruli/lib

WARN = -pedantic -ansi -Wshadow -Wpointer-arith -Wcast-qual \
        -Wcast-align -Wwrite-strings -Wstrict-prototypes \
        -Wmissing-prototypes -Wredundant-decls \
	-Wall \
#	-Werror
CFLAGS = $(WARN) -O2 -pipe -g -shared -D_REENTRANT -D__USE_GNU \
	-fPIC $(shell guile-config compile) $(INCLUDES) $(LIBS)
LDFLAGS = $(LIBS) -lruli $(shell guile-config link)
CC = gcc

.PHONY: default
default: ruli-guile

.PHONY: clean
clean:
	rm -f *.o *.so *~ ruli-guile

.PHONY: build
build: clean default

ruli-guile: ruli-guile.o libguile-ruli.so

libguile-ruli.so: guile_ruli.o
	gcc -shared -o $@ $< $(LDFLAGS)
