# Makefile for homework 4

OBJS = listtest.o list.o

CFLAGS = -g -Wall

hw4: ${OBJS}
	@echo
	@echo Linking program $@
	gcc -o $@ ${OBJS}

${OBJS}: list.h Makefile

clean:
	rm -f *.o  hw4

