CC = g++
CFLAGS = -g -Wall

OBJS=lab11test.o ray.o

lab11: ${OBJS}
	${CC} -o $@ ${OBJS} -g

.cpp.o:
	${CC} -c ${CFLAGS} $< -o $@

${OBJS}: ray.h Makefile

clean:
	rm -f *.o lab11
