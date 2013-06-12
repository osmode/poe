CC = g++
CFLAGS = -c -Wall -g
LDFLAGS = 

all: poe

poe: poe.o hashtable.o listtools.o
	$(CC) -o poe poe.o $(LDFLAGS)

poe.o: poe.cpp
	$(CC) $(CFLAGS) poe.cpp

hashtable.o: hashtable.cpp
	$(CC) $(CFLAGS) hashtable.cpp

listtools.o: listtools.cpp
	$(CC) $(CFLAGS) listtools.cpp

clean: 
	rm -rf *o poe hashtable listtools 


