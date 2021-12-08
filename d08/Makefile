CC=gcc
# CFLAGS=-O3
CFLAGS=-g -Wall
DBGCFLAGS=-g -Wall -DDBG
DBG2CFLAGS=-g -Wall -DDBG2
TDBGCFLAGS=-g -Wall -DDBG # True debug flags!

LIBS=-lgsl -lgslcblas -lm
EXES=grd

grd: grd.c
	${CC} ${DBGCFLAGS} -o $@ $^

.PHONY: clean

clean:
	rm -f ${EXES}
