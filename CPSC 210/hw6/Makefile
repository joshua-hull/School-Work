# Makefile for Homework 6
CC = gcc
CFLAGS = -g -Wall

OBJS= main.o       \
      scene.o      \
      entity.o     \
      sceneobj.o   \
      plane.o      \
      sphere.o     \
      list.o       \
      image.o      \
      readVals.o   \
      window.o     \
      tuple.o

#      raytrace.o   \

INCS=   ray.h        \
        scene.h      \
        window.h     \
        image.h      \
        list.h       \
        entity.h     \
        sceneobj.h   \
        plane.h      \
        sphere.h     \
        tuple.h      \
        readVals.h 

hw6: ${OBJS}
	@echo
	@echo Linking $@
	gcc -o $@ -Wall -g ${OBJS} -lm

${OBJS}: ${INCS} Makefile

test:
	./hw6 scene.txt

clean:
	rm -f *.o  hw6
