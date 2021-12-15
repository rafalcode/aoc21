CC=gcc
# CFLAGS=-O3
CFLAGS=-g -Wall
DBGCFLAGS=-g -Wall -DDBG
DBG2CFLAGS=-g -Wall -DDBG2
TDBGCFLAGS=-g -Wall -DDBG # True debug flags!

EXES=p1 p2 p3 p1a p1b rdin rd2

rdin: rdin.c
	${CC} ${DBGCFLAGS} -o $@ $^
rd2: rd2.c
	${CC} ${DBGCFLAGS} -o $@ $^
p1: p1.c
	${CC} ${DBGCFLAGS} -o $@ $^
p1a: p1a.c
	${CC} ${DBGCFLAGS} -o $@ $^
p1b: p1b.c
	${CC} ${DBGCFLAGS} -o $@ $^
grd2: grd2.c
	${CC} ${DBGCFLAGS} -o $@ $^
p2: p2.c
	${CC} ${DBGCFLAGS} -o $@ $^
p3: p3.c
	${CC} ${DBGCFLAGS} -o $@ $^

.PHONY: clean

clean:
	rm -f ${EXES}
