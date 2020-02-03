CC = g++
CFLAGS = -g -c
OBJECTS = Assign1.o 

all: Assign1 ProfProg
Assign1: $(OBJECTS) 
						$(CC) $(OBJECTS) -o Assign1 $(LIBS)
Assign1.o: Assign1.cpp 
						$(CC) $(CFLAGS)  Assign1.cpp
ProfProg: ProfProg.o
						$(CC) ProfProg.o -o ProfProg $(LIBS)
ProfProg.o: ProfProg.cpp 
						$(CC) $(CFLAGS)  ProfProg.cpp
clean:
						rm -f Assign1 ProfProg ProfProg.o $(OBJECTS) Assign1.hpp.gch
