CFLAGS= -g -Wall
CC= gcc

OBJS= lab9test.o carlist.c

lab9: ${OBJS}
	@echo Linking $@
	gcc -o $@ ${OBJS} -g

${OBJS}: carlist.h Makefile

clean:
	rm -f *.o lab9
