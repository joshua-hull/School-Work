CC=gcc
CFLAGS=-g -Wall
LDFLAGS=
SOURCES=lab3test.c mystrings.c
EXECUTABLES=lab3

all: $(EXECUTABLES)

lab3: $(SOURCES) 
	$(CC) -o $(EXECUTABLES) $(SOURCES) $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf $(EXECUTABLES)
