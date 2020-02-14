CC = g++
CFLAGS = -g -c
OBJECTS = Assign2_grep.o 

all: Assign2_grep Assign2_sort
Assign2_grep: $(OBJECTS) 
						$(CC) $(OBJECTS) -o Assign2_grep $(LIBS)
Assign2_grep.o: Assign2_grep.cpp 
						$(CC) $(CFLAGS)  Assign2_grep.cpp
Assign2_sort: Assign2_sort.o  
						$(CC) Assign2_sort.o  -o Assign2_sort $(LIBS)
Assign2_sort.o: Assign2_sort.cpp 
						$(CC) $(CFLAGS)  Assign2_sort.cpp
clean:
						rm -f Assign2_grep  $(OBJECTS) Assign2_grep.hpp.gch Assign2_sort Assign2_sort.o Assign2_sort.gch
						clear
