# Cpsc 210: Lab 8: Makefile (c) 2012 Joshua Hull
# Makefile for Lab 8
CC = gcc
CFLAGS = -g -Wall
OBJS = lab8main.o plane.o tuple.o
INCS = ray.h image.h entity.h list.h scene.h sceneobj.h plane.h tuple.h

lab8: ${OBJS}
	@echo
	@echo Linking $@
	-gcc -o $@ -Wall -g ${OBJS} -lm
    
${OBJS}: ${INCS} Makefile
clean:
	rm -f *.o lab8
test1:
	./lab8 0 0 4
