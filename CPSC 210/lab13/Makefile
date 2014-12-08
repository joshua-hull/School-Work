# Makefile for lab 13 (templates)

CC= g++
CXXFLAGS= -g -Wall
OBJS= deque.o

lab13: ${OBJS}
	@echo Linking lab13
	${CC} -o $@ -g ${OBJS}

test:	
	./lab13

clean:
	rm -f ${OBJS} lab13
