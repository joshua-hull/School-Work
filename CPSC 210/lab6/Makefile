# Makefile for lab6
OBJS=nibbletest.o nibbles.o
CFLAGS=-Wall -g

lab6: ${OBJS}
	gcc -o $@ -g ${OBJS}

${OBJS}: nibbles.h Makefile

test:
	./lab6

clean:
	rm -f *.o lab6


