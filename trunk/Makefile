CC=g++
LDFLAGS=
CFLAGS=-pipe -Wall -ansi -pedantic -Werror -O3  
#CFLAGS=-pipe -Wall -ansi -pedantic -g -O1
OBJECTS=sudoku.o
BIN=yasss

.SUFFIXES: .cpp 

.cpp.o: 
	$(CC) $(CFLAGS) -c $< 


$(BIN): $(OBJECTS) main.cpp sudoku.hpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN) main.cpp $(OBJECTS)


clean:
	rm -f *.o $(BIN) core

test: $(BIN)
	./ok_tests.sh
	echo "Tests finished sucessfully"
