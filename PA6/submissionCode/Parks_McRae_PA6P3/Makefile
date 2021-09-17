# Makefile for UD CISC user-level thread library

CC = gcc
CFLAGS = -g

LIBOBJS = t_lib.o 

TSTOBJS = test00.o philosophers.o 3test.o order.o rendezvous.o

# specify the executable 

EXECS = test00 philosophers 3test partialorder rendezvous

# specify the source files

LIBSRCS = t_lib.c

TSTSRCS = test00.c

# ar creates the static thread library

t_lib.a: ${LIBOBJS} Makefile
	ar rcs t_lib.a ${LIBOBJS}

# here, we specify how each file should be compiled, what
# files they depend on, etc.

t_lib.o: t_lib.c t_lib.h Makefile
	${CC} ${CFLAGS} -c t_lib.c

test00.o: test00.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c test00.c

test00: test00.o t_lib.a Makefile
	${CC} ${CFLAGS} test00.o t_lib.a -o test00

philosophers.o:	philosophers.c	ud_thread.h Makefile
	${CC} ${CFLAGS} -c philosophers.c

philosophers: philosophers.o t_lib.a Makefile
	${CC} ${CFLAGS} philosophers.o t_lib.a -o philosophers

3test.o: 3test.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c 3test.c

3test:	3test.o t_lib.a Makefile
	${CC} ${CFLAGS} 3test.o t_lib.a -o 3test

order.o: order.c t_lib.a Makefile
	${CC} ${CFLAGS} -c order.c

order: order.o t_lib.a Makefile
	${CC} ${CFLAGS} order.o t_lib.a -o partialorder

rendezvous.o: rendezvous.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c rendezvous.c

rendezvous: rendezvous.o t_lib.a Makefile
	${CC} ${CFLAGS} rendezvous.o t_lib.a -o rendezvous

all: test00 philosophers 3test order rendezvous

clean:
	rm -f t_lib.a ${EXECS} ${LIBOBJS} ${TSTOBJS} 
