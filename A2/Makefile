CC = g++
CFLAGS = -g -c
OBJECTS = Assign2.o 

all: Assign2 
Assign2: $(OBJECTS) 
						$(CC) $(OBJECTS) -o Assign2 $(LIBS)
Assign2.o: Assign2.cpp 
						$(CC) $(CFLAGS)  Assign2.cpp
clean:
						rm -f Assign2  $(OBJECTS) Assign2.hpp.gch
