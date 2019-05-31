#
# $Id: Makefile,v 1.2 2004/10/26 21:23:01 evertonm Exp $
#

INCLUDES=-I/usr/local/j2sdk/include \
	-I/usr/local/j2sdk/include/linux \
	-I/usr/local/ruli/include

LIBS=-L/usr/local/ruli/lib

CFLAGS = -Wall -O2 -pipe -g -shared -D_REENTRANT -D__USE_GNU \
	-fPIC $(INCLUDES) $(LIBS)
LDFLAGS = -lruli $(LIBS)
CC = gcc

.PHONY: default
default: java-ruli

.PHONY: clean
clean:
	rm -f *.o *.so *~

.PHONY: build
build: clean default

java-ruli: libjava-ruli.so

libjava-ruli.so: ruli_RuliSyncImp.o
	gcc -shared -o $@ $< $(LDFLAGS)
