#
# $Id: php_ruli.make,v 1.2 2004/06/03 19:17:44 evertonm Exp $
#

.PHONY: default
default: php_ruli.so

.PHONY: clean
clean:
	rm -f *.o *.so *.lo

.PHONY: build
build: clean default

php_ruli.o: php_ruli.c
	gcc -Wall -fPIC -DCOMPILE_DL_RULI=1 -I/usr/local/ruli/include -I/usr/local/include -I. -I.. -I../.. -I../../Zend -I../../main -I../../TSRM -c php_ruli.c

php_ruli.so: php_ruli.o
	gcc -Wall -shared -L/usr/local/ruli/lib -L/usr/local/lib -rdynamic -o php_ruli.so php_ruli.o

