# Makefile for ray tracer 3
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
      render.o     \
      raytrace.o   \
      tuple.o      \
      pointlight.o \
      lighting.o   \
      triangle.o   \
      edge.o       \
      polyhit.o    \
      rectangle.o  \
      ring.o       \
      tiled.o

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
        render.h     \
        raytrace.h   \
        readVals.h   \
        pointlight.h \
        lighting.h   \
        triangle.h   \
        edge.h       \
        polyhit.h    \
        rectangle.h  \
        ring.h       \
        tiled.h

ray3: ${OBJS}
	@echo
	@echo Linking $@
	gcc -o $@ -Wall -g ${OBJS} -lm

${OBJS}: ${INCS} Makefile

test:
	./ray3 scene.txt output.ppm
	display output.ppm

clean:
	rm -f *.o  ray3
