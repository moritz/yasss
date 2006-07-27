/* field.hpp
 * a sudoku playing field.
 * 
 * Copyright (C) Moritz Lenz <moritz@faui2k3.org> 2005
 *
 * Part of the sudoku Projekt.
 * License: take a look at main.cpp (in short: GPL or BSD artistic, whatever
 * you like)
 */

#ifndef _MORITZ_FIELD_
#define _MORITZ_FIELD_
#include <iostream>

// a sudoku playing field implemented as a 2d fixed size array
// contains consitency checks and a solver.
class sudoku {
	public:
		sudoku();
		// creates a field with inital data. 0 means "not set".
		// Note that the first coordinate ist considered as x, so if
		// you create an array char f= {{1 ,2 ...}, {..}} you will get
		// the transpoesed sudoku field. but don't worry, sudoku is
		// invariant under transposition
		sudoku(char init_data[9][9]);
		sudoku(char* init_data);

		// creates a field with inital data. 0 means "not set".
		// Note that the first coordinate ist considered as x, so if
		// you create an array char f= {{1 ,2 ...}, {..}} you will get
		// the transpoesed sudoku field. but don't worry, sudoku is
		// invariant under transposition
		sudoku(int init_data[9][9]);

		// generates a rather simplistic output to the given stream
		// call as pretty_print(cout) or something like that...
		void pretty_print(std::ostream &handle);

		// just print all chars in one row
		void print(std::ostream &handle);

		// sets item (x, y) to val
		// assumes that it is doesn't lead to an intermediate
		// confilict with sudoku rules
		// which is equivalent to saying it requires 
		// allowed_set(val, x, y) to be true
		void set_item(char val, int x, int y);

		// get entry at position (x, y)
		// 0 means "unset"
		int get_item(int x, int y);
		
		// returns true if it doesn't lead to a direct error if you
		// set (x, y) to val
		// If data[x][y] != 0 the return value is 
		// true if val == data[x][y]
		bool allowed_set(char val, int x, int y);

		// try to solve the puzzle. Returns true on success.
		bool solve();
		
		// returns true if there is no zero entry left, e.g. the
		// problem is solved correctly.
		bool is_solved();

		// returns true if there is no possibilty to continue without
		// violating rule
		bool is_stuck();
	protected:

		// contains 0 for unset values and the corresponding value 
		// if the value is set
		char data[9][9];

		// allowed[x][y][i] is true if and only if it is possible to
		// set data[x][y] to i+1 without conjuring an immediate
		// collision.
		// If data[x][y] == i != 0 then allowed[x][y][i] is true,
		// allowed[x][y][j] = false for j != i
		bool allowed[9][9][9]; 
		bool simple_solve();
		bool simple_solve1();
		bool simple_solve2();
		// returns either an is_solved or a stuck() version of *this
		bool backtrack();
		void null_init();

		int recursion_depth;
		void set_recursion_depth(int rd) {recursion_depth = rd;};
};

#endif /* _MORITZ_FIELD */

