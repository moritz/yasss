/* sudoku.cpp
 * a sudoku playing field, implementation of sudoku.hpp
 * 
 * Copyright 2005, 2006 (C) Moritz Lenz <moritz@faui2k3.org> 
 *
 * Part of the sudoku Projekt.
 * License: take a look at main.cpp (in short: GPL or BSD License, whatever
 * you like)
 */

#include "sudoku.hpp"
#include "perm.h"
#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

using std::cerr;

sudoku::sudoku(){
	null_init();
	init();
}

sudoku::sudoku(char init_data[9][9]){
	null_init();
	init();
	for(int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			// TODO: error handling instead of assertions
			assert(init_data[x][y] >= 0);
			assert(init_data[x][y] <= 9);
			if (init_data[x][y] > 0){
				if (allowed_set(init_data[x][y], x, y)){
					set_item(init_data[x][y], x, y);
				} else {
					cerr << "The given Sudoku is errornous, ignoring conflicting numbers...\n";
				}
			} // if
		} // for y
	} // for x
}


sudoku::sudoku(char* init_data){
	null_init();
	for (int i = 0; i < 81; i++){
		int x = i % 9;
		int y = i / 9;
		assert(init_data[i] <= 9);
		assert(init_data[i] >= 0);
		if(allowed_set(init_data[i], x, y)){
			set_item(init_data[i], x, y);
		} else {
			cerr << "The given Sudoku is errornous, ignoring conflicting numbers...\n";
		}
	}
}


sudoku::sudoku(int init_data[9][9]){
	null_init();
	init();
	for(int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			// TODO: error handling instead of assertions
			assert(init_data[x][y] >= 0);
			assert(init_data[x][y] <= 9);
			if (init_data[x][y] > 0){
				if (allowed_set((char)init_data[x][y], x, y)){
					set_item((char)init_data[x][y], x, y);
				} else {
					cerr << "The given Sudoku is errornous, ignoring conflicting numbers...\n";
				}
			} // if
		} // for y
	} // for x
}

int sudoku::get_item(int x, int y){
	return (int) data[x][y];
}



void sudoku::set_item(char val, int x, int y){
	assert(allowed_set(val, x, y));
	assert(data[x][y] == 0);
	assert(val >= 1);
	assert(val <= 9);

	data[x][y] = val;


	// update allowed configurations

	// set allowed[][][val-1] = false in rows and cols
	for(int i = 0; i < 9; i++){
		allowed[x][i][val - 1] = false;
		allowed[i][y][val - 1] = false;
	}

	// now take care of the apropriate 3x3 block
	int x_orig = 3 * (int) (x/3);
	int y_orig = 3 * (int) (y/3);
	for (int i = x_orig; i < x_orig + 3; i++){
		for (int j = y_orig; j < y_orig + 3; j++){
			allowed[i][j][val - 1] = false;
		}
	}

	for (int i = 0; i < 9; i++)
		allowed[x][y][i] = false;

}

void sudoku::pretty_print(std::ostream &handle){
	for (int i = 0; i < 9; i++){
		handle<<(int)data[0][i]<<" "<<(int)data[1][i]<<" "
			<<(int)data[2][i]<<" "<<(int)data[3][i]<<" "
			<<(int)data[4][i]<<" "<<(int)data[5][i]<<" "
			<<(int)data[6][i]<<" "<<(int)data[7][i]<<" "
			<<(int)data[8][i]<<"\n";
	}
}

void sudoku::print(std::ostream &handle){
	for (int y = 0; y < 9; y++){
		for (int x = 0; x < 9; x++){
			handle << (int) data[x][y];
		}
	}
	handle << std::endl;
}

bool sudoku::solve(){
	simple_solve();
	if (is_stuck()){
		return false;
	}
	if (is_solved()){
		solution_count ++;
		if (solution_count % 100000 == 0){
			cerr << "Current number of Solutions: "
				<< solution_count << "\n";
		}
		return true;
	} else {
		return backtrack();
	}
}

bool sudoku::simple_solve(){
	bool res = false;
	bool flag = true;
	while (flag){
		flag = simple_solve1();
		flag = simple_solve2() || flag;
		if (!calculate_difficulty_rating){
			simple_solve3() || flag;
		}
		if (flag){
			res = true;
		}
	} // while
	return res;
} // function

bool sudoku::simple_solve1(){
	// looks at every place in the lattice if there is a only one number
	// possible. If so, the apropriate number is set.
	bool res = false;
	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			if ((int) data[x][y] == 0){
				int c = 0;
				int i0 = 0;
				for (int i = 0; i < 9; i++){
					if (allowed[x][y][i]){
						i0 = i;
						c++;
					}
				}
				if (c == 1){
					set_item(i0 + 1, x, y);
					difficulty_rating ++;
					res = true;

				} // for i

			} // if (data != 0)
		} // for y
	} // for x
	return res;
}

bool sudoku::is_solved(){
	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			if (data[x][y] == 0){
				return false;
			}
		}
	}
	return true;
}

void sudoku::null_init(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			data[i][j] = 0;
			for (int k = 0; k < 9; k++){
				allowed[i][j][k] = true;
			}
		}
	}
}

void sudoku::init(){
	recursion_depth = 0;
	count_solutions = false;
	test_if_uniq = false;
	solution_count = 0;
	calculate_difficulty_rating = false;
	difficulty_rating = 0;

}

bool sudoku::simple_solve2(){
	// searches if there is any row/column/3x3-square where there is a
	// number which can be placed in only one position.
	// If it finds such a number it is set.
	bool res = false;
	for (int x = 0; x < 9; x++){
		int vcount[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
		for (int y = 0; y < 9; y++){
			if (data[x][y] == 0) {
				for (int i = 0; i < 9; i++){
					if (allowed[x][y][i]){
						vcount[i]++;
					}
				}
			} else {
				// any numer > 1 will do...
				vcount[data[x][y] - 1] = 10;
			} // if
		} // for y
		for (int i = 0; i < 9; i++){
			if (vcount[i] == 1){
				// Hurray
				// We know that there is only one possible
				// place for number i+1 to put
				// we've got to find it:
				for (int k = 0; k < 9; k++){
					if (allowed[x][k][i]){
						set_item(i+1, x, k);
						difficulty_rating ++;
					}
				}
				res = true;
			}
		} // for i
	} // for x



	// now we've got to do the hole damn thing for the columns:
	for (int y = 0; y < 9; y++){
		int hcount[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
		for (int x = 0; x < 9; x++){
			if (data[x][y] == 0) {
				for (int i = 0; i < 9; i++){
					if (allowed[x][y][i]){
						hcount[i]++;
					}
				}
			} else {
				// any numer > 1 will do...
				hcount[data[x][y] - 1] = 10;
			} // if
		} // for y
		for (int i = 0; i < 9; i++){
			if (hcount[i] == 1){
				// Hurray
				// We know that there is only one possible
				// place for number i+1 to put
				// we've got to find it:
				for (int k = 0; k < 9; k++){
					if (allowed[k][y][i]){
						set_item(i+1, k, y);
						difficulty_rating ++;
						res = true;
					}
				}
			}
		} // for i
	} // for x


	// now it's getting really nasty. Do the same for the 3x3-subsquares:
	// (It's not nasty in principle, you just have to be carefull not to
	// counfound some indices)
	for (int k = 0; k < 9; k++){
		int xb = 3 * ((int) k / 3);
		int yb = 3 * ( k % 3);
		int rcount[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
		for (int i = 0; i < 9; i++){
			int x = xb + i % 3;
			int y = yb + (int) i/3;
			if (data[x][y] == 0){
				for (int k = 0; k < 9; k++){
					if (allowed[x][y][k]){
						rcount[k] ++;
					}
				} // for k
			} else {
				rcount[data[x][y] - 1] = 10;
			} // if data == 0
		} // for i
		for (int i = 0; i < 9; i++){
			if (rcount[i] == 1){
				for (int k = 0; k < 9; k++){
					int x = xb + k % 3;
					int y = yb + (int) k/3;
					if (allowed[x][y][i]){
						set_item(i+1, x, y);
						difficulty_rating ++;
						res = true;
					}
				}
			} // if rcount[i] == 1
		} // for i
	}

	return res;
} // function

bool sudoku::simple_solve3(){
	bool res = false;
	for (int i = 0; i < 9; i++){
		int base_x = 3 * ((int) (i / 3));
		int base_y = 3 * (i % 3);
		for (int n = 1; n < 10; n++){
			bool row[3] = {false, false, false};
			bool col[3] = {false, false, false};
			for (int k = 0; k < 9; k++){
				int y =  (int) (k / 3);
				int x =  k % 3;
				if (allowed_set(n, base_x + x, base_y + y)){
					col[x] = true;
					row[y] = true;
				}
			}
			int r = -1;
			int c = -1;
			if ( row[0] && !row[1] && !row[2]) r = 0;
			if (!row[0] &&  row[1] && !row[2]) r = 1;
			if (!row[0] && !row[1] &&  row[2]) r = 2;

			if ( col[0] && !col[1] && !col[2]) c = 0;
			if (!col[0] &&  col[1] && !col[2]) c = 1;
			if (!col[0] && !col[1] &&  col[2]) c = 2;
			if (c > -1){
				int x = c + base_x;
				assert(col[c]);
				for (int y = 0; y < 9; y++){
					if ((y < base_y || y > base_y + 2)){
						res |= allowed[x][y][n-1];
						allowed[x][y][n -1] = false;
					}
				}
			}
			if (r > -1){
				int y = r + base_y;
				assert(row[r]);
				for (int x = 0; x < x; y++){
					if ((x < base_x || x > base_x + 2)){
						res |= allowed[x][y][n-1];
						allowed[x][y][n -1] = false;
					}
				}
			}


		}
	}
	return res;

}

bool sudoku::is_stuck(){
	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			if (data[x][y] == 0){
				bool c = false;
				for (int i = 0; i < 9; i++){
					c |= allowed[x][y][i];
				}
				if (!c){
					return true;
				}
			}
		}
	}
	return false;
}

bool sudoku::backtrack(){
	if (is_stuck()){
		return false;
	}
//	cerr << "Recursion depth: " << recursion_depth << "\n";
	difficulty_rating += 7;
	sudoku solution;
	bool is_solved = false;
	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			if (data[x][y] == 0){
				for (int i = 0; i < 9; i++){
					if (allowed[x][y][i]){
						sudoku tmp = *this;
						tmp.set_item(i+1, x, y);
						tmp.set_recursion_depth(recursion_depth + 1);
						bool res = tmp.solve();
						difficulty_rating = tmp.get_difficulty_rating();
						if (res){
							is_solved = true;
							if (count_solutions){
								solution_count = tmp.get_solution_count();
								if (test_if_uniq && solution_count > 1){
									return true;
								}
								solution = tmp;
							} else {
								*this = tmp;
								return true;
							}
						}
					}
				}
				if (is_solved){
					*this = solution;
				}
				return is_solved;
			}//  if data == 0
		} // for y
	} // for x
	return false;
	}


int sudoku::get_solution_count() {
	return solution_count;
} 

void sudoku::print_mask(int n){
	for(int y = 0; y < 9; y++){
		for (int x = 0; x < 9; x++){
			if (allowed[x][y][n-1]){
				cerr << "X ";
			} else {
				cerr << "  ";
			}
		}
		cerr << "\n";
	}

}

int sudoku::count_entries(){
	int c = 0;
	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			if (data[x][y] !=0){
				c++;
			}
		}
	}
	return c;
}

void sudoku::add_random_number(){
		int i = random() % 81;
		int x = i % 9;
		int y = i / 9;
		int n = random() % 9;
		if (allowed[x][y][n]){
			set_item(n + 1, x, y);
		}
}

void sudoku::random_generate(void){
	{
		// init random number generator with microseconds since begin
		// of the epoch:
		timeval tv;
		gettimeofday(&tv, NULL);

		srand(1000000*tv.tv_sec + tv.tv_usec);
	}
	int tries = 0;
	while (true){
		while (count_entries() < 27){
			add_random_number();

		}
		int count = 0;
		do {
			add_random_number();
			sudoku tmp = *this;
			tmp.count_solutions = true;
			tmp.test_if_uniq = true;
			tmp.solve();
			count = tmp.get_solution_count();
//			cerr << count << "\n";
			if (count == 1){
//				cerr << "Discarded tries: " << tries << "\n";
				return;
			}
		} while (count > 1);
		tries++;
		// attempt failed
		null_init();
	}
}

void sudoku::to_canonical_form(void){
	char* least = new char[81];
	for (int i = 0; i < 81; i++){
		least[i] = 9;
	}
	char transposed[9][9];
	for (int y = 0; y < 9; y++){
		for (int x = 0; x < 9; x++){
			transposed[y][x] = data[x][y];
		}
	}
	for (int i = 0; i < 1296; i++){
		for(int j = 0; j < 1296; j++){
			compare_and_update(least, data, i, j);
			compare_and_update(least, transposed, i, j);
		}
	}
	null_init();
	for (int i = 0; i < 81; i++){
		int x = i % 9;
		int y = i / 9;
		if (least[i] > 0){
			set_item(least[i], x, y);
		}

	}
}

void sudoku::perm_copy(char source[9][9], char* dest, int i, int j){
	// copy source to dest.
	// Applay transformations (i, j) before copying
	// Generate a minimal permutation on the fly.
	short int map[10] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	int next_map = 1;
	for(int k = 0; k < 81; k++){
		int a = (int) source[perm[i][k % 9]][perm[j][k / 9]];
		if (map[a] == -1){
			map[a] = next_map;
			next_map++;
		}
		dest[k] = map[a];
	}
}

void sudoku::compare_and_update (char* least, char tmp[9][9], int i, int j){
	// apply transformation (i, j) to `tmp';
	// then compare the minimal permutation of `tmp' to `least'
	// if the minimal permuatation is smaller then `least', update `least'
	int count = 0;
	short int map[10] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	short int next_map = 1;
	for (int y = 0; y <9; y++){
		for (int x = 0; x < 9; x++){
			int a = (int) tmp[perm[i][x]][perm[j][y]];
			if (map[a] == -1){
				map[a] = next_map;
				next_map++;
			}
			if (map[a] < least[count]){
				perm_copy(tmp, least, i, j);
				return;
			} else if (map[a] > least[count]){
				return;
			}
			count ++;
		}
	}

}
