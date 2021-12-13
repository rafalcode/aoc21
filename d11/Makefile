CC=gcc
# CFLAGS=-O3
CFLAGS=-g -Wall
DBGCFLAGS=-g -Wall -DDBG
DBG2CFLAGS=-g -Wall -DDBG2
TDBGCFLAGS=-g -Wall -DDBG # True debug flags!

LIBS=-lgsl -lgslcblas -lm
EXES=p1 grd2 p2 p3 p4 p5

p1: p1.c
	${CC} ${DBGCFLAGS} -o $@ $^
grd2: grd2.c
	${CC} ${DBGCFLAGS} -o $@ $^
p2: p2.c
	${CC} ${DBGCFLAGS} -o $@ $^
p3: p3.c
	${CC} ${DBGCFLAGS} -o $@ $^
p4: p4.c
	${CC} ${DBGCFLAGS} -o $@ $^
p5: p5.c
	${CC} ${DBGCFLAGS} -o $@ $^

.PHONY: clean

clean:
	rm -f ${EXES}
