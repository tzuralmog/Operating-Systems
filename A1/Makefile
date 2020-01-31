CC = g++
CFLAGS = -g -c
OBJECTS = Assign1.o 

all: Assign1
Assign1: $(OBJECTS) 
						$(CC) $(OBJECTS) -o Assign1 $(LIBS)
Assign1.o: Assign1.cpp 
						$(CC) $(CFLAGS)  Assign1.cpp
clean:
						rm -f Assign1 $(OBJECTS) Assign1.hpp.gch
