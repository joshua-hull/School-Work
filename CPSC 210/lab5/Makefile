# Makefile for lab 5

OBJS = listtest.o list.o

CFLAGS = -g -Wall

lab5: ${OBJS}
	@echo
	@echo Linking program $@
	gcc -o $@ ${OBJS}

${OBJS}: list.h Makefile

clean:
	rm -f *.o  lab5

