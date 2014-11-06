CC = g++

LDFLAGS =
CFLAGS = -g 

OBJECTS = Matrix.o Utility.o Vector.o

all: ${OBJECTS}

%.o: %.cpp
	${CC} -c ${CFLAGS} *.cpp

clean:
	rm -f ${OBJECTS}
