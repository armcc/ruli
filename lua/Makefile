#
# $Id: Makefile,v 1.2 2004/10/26 21:28:14 evertonm Exp $
#

INCLUDES=-I../src -I/usr/local/ruli/include
LIBS=-L../src -L/usr/local/ruli/lib

WARN = -pedantic -ansi -Wshadow -Wpointer-arith -Wcast-qual \
        -Wcast-align -Wwrite-strings -Wstrict-prototypes \
        -Wmissing-prototypes -Wredundant-decls \
	-Wall \
#	-Werror
CFLAGS = $(WARN) -O2 -pipe -g -shared -D_REENTRANT -D__USE_GNU \
	-fPIC $(shell lua-config --include) $(INCLUDES) $(LIBS)
LDFLAGS = $(LIBS) -lruli -llua50
CC = gcc

.PHONY: default
default: liblua-ruli.so

.PHONY: clean
clean:
	rm -f *.o *.so *~ 

.PHONY: build
build: clean default

liblua-ruli.so: lua_ruli.o
	gcc -shared -o $@ $< $(LDFLAGS)
