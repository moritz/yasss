CC=g++
LDFLAGS=
#CFLAGS=-pipe -Wall -ansi -pedantic -Werror -Os -march=i686 
CFLAGS=-pipe -Wall   -g -O1
OBJECTS=sudoku.o
BIN=yasss

.SUFFIXES: .cpp 

.cpp.o: .hpp
	$(CC) $(CFLAGS) -c $< 


$(BIN): $(OBJECTS) main.cpp sudoku.hpp Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN) main.cpp $(OBJECTS)

gui: gui.cpp sudoku.cpp sudoku.hpp perm.h
	g++ $(CFLAGS) gui.cpp sudoku.o `pkg-config --cflags --libs gtk+-2.0` -o gui


yasss-curses: tui.cpp sudoku.o sudoku_nosane.o
	g++ $(CFLAGS)  -lncurses tui.cpp sudoku.o sudoku_nosane.o -o yasss-curses

clean:
	rm -f *.o $(BIN) core

tests: $(BIN)
	cd test && ./ok_tests.sh
	cd test && ./count_tests.sh
	cd test && ./canonical_tests.sh
	echo "Tests finished sucessfully"

html-doc: yasss.1.txt
	asciidoc -d manpage yasss.1.txt
	asciidoc -d manpage yasss-curses.6.txt
manpage: yasss.1.txt
	a2x -f manpage yasss.1.txt
	a2x -f manpage yasss-curses.6.txt
