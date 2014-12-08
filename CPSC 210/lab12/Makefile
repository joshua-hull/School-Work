# Make file for lab 12

CPPFLAGS= -g -Wall
OBJS= lab12test.o tree.o

lab12: ${OBJS}
	g++ -o $@ -g ${OBJS}

${OBJS}: tree.h Makefile

clean:
	rm -f *.o lab12

test:
	./lab12
