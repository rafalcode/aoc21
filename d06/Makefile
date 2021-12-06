CC=gcc
CFLAGS=-g -Wall
EXECUTABLES=a b c

a: a.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
b: b.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
c: c.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

.PHONY: clean

clean:
	rm -f ${EXECUTABLES}
