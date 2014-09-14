CC	= g++
CFLAGS = -g -I/group/dpa/include

LDFLAGS = -L/usr/lib64/ -L/group/dpa/lib -lglut -lGL -lGLU -lOpenImageIO -lm

PROJECT	= albers

OBJECTS = main.o project.o colorwindow.o record.o 

${PROJECT}: ${OBJECTS}
	${CC} -o ${PROJECT} ${OBJECTS} ${LDFLAGS}

%.o: %.cpp
	${CC} -c ${CFLAGS} *.cpp

clean:
	rm -f core.* *.o *~ ${PROJECT}
