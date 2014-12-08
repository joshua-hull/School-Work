CFLAGS=-g -Wall
OBJS=lab7test.o double.o

lab7: ${OBJS}
	gcc -o $@ -g ${OBJS}

${OBJS}: dlist.h

clean:
	rm -f *.o lab7

test:
	./lab7
