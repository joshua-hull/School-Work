OBJS = testmain.o mylist.o stack.o queue.o mynode.o

CXXFLAGS = -g -Wall

all: lab14

lab14: ${OBJS}
	g++ -o $@ ${OBJS}

${OBJS}: mylist.h mynode.h stack.h queue.h Makefile

clean:
	rm -f *.o lab14

test:
	./lab14
