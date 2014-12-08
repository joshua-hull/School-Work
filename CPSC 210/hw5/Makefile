# Makefile for ray tracer 3
CC = gcc
CFLAGS = -g -Wall

OBJS= hw5test.o    \
      scene.o      \
      entity.o     \
      list.o       \
      image.o      \
      readVals.o   \
      window.o     \
      raytrace.o   \
      tuple.o

#      raytrace.o   \

INCS=   ray.h        \
        scene.h      \
        window.h     \
        image.h      \
        list.h       \
        entity.h     \
        tuple.h      \
        raytrace.h   \
        readVals.h 

hw5: ${OBJS}
	@echo
	@echo Linking $@
	gcc -o $@ -Wall -g ${OBJS} -lm

${OBJS}: ${INCS} Makefile

test:
	./hw5 < testdata.txt

clean:
	rm -f *.o  hw5
