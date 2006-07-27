// Yass: Yat Another Stupid Sudoku Solver
//
// Written by Moritz Lenz <moritz at faui2k3.org>
//
// This program is free software. You may redistribute and change it under the
// terms of the GNU General Public License Version 2 (June 1991) as published
// by the Free Software Fundation, Inc., 59 Temple Place, Suite 330, Boston,
// MA 02111-1307 USA
// or under the terms of the BSD Artistic License.
// Chosse the one you like better.
//
// Yass solves the sudoku game (see http://en.wikipedia.org/wiki/Sudoku if you
// don't know what it is).
// It simply reads a field fron standard input, numbers separated by
// whitespaces.
// It solves it (if possible), and prints the solved version in the same
// format.
//
#include <iostream>
#include "sudoku.hpp"
#include <assert.h>
using namespace std;


int main(int argc, char** argv){
	// some possible test data, commented out
	// increasing difficoulty
/*	char f[9][9]= {{5, 3, 0, 0, 7, 0, 0, 0, 0},
			{6, 0, 0, 1, 9, 5, 0, 0, 0},
			{0, 9, 8, 0, 0, 0, 0, 6, 0},
			{8, 0, 0, 0, 6, 0, 0, 0, 3},
			{4, 0, 0, 8, 0, 3, 0, 0, 1},
			{7, 0, 0, 0, 2, 0, 0, 0, 6},
			{0, 6, 0, 0, 0, 0, 2, 8, 0},
			{0, 0, 0, 4, 1, 9, 0, 0, 5},
			{0, 0, 0, 0, 8, 0, 0, 7, 9}};
			*/
/*	char f[9][9] = {{0, 8, 0, 0, 7, 0, 0, 3, 0},
			{0, 5, 0, 4, 0, 0, 0, 0, 0},
			{1, 0, 0, 6, 0, 0, 9, 0, 0},
			{3, 1, 0, 0, 0, 0, 8, 0, 0},
			{6, 0, 0, 0, 5, 0, 0, 0, 2},
			{0, 0, 9, 2, 0, 0, 0, 4, 6},
			{0, 0, 3, 0, 0, 9, 0, 0, 1},
			{0, 0, 0, 0, 0, 1, 0, 6, 0},
			{0, 9, 0, 0, 4, 0, 0, 2, 0}};*/
/*	char f[9][9] = {{0, 0, 5, 0, 0, 8, 0, 1, 0},
			{8, 0, 0, 0, 0, 6, 0, 0, 0},
			{0, 0, 3, 0, 0, 0, 4, 0, 0},
			{0, 0, 6, 0, 2, 0, 0, 3, 0},
			{2, 0, 0, 0, 0, 9, 0, 0, 7},
			{0, 4, 0, 0, 0, 0, 1, 0, 0},
			{0, 0, 1, 0, 0, 0, 3, 0, 0},
			{0, 0, 0, 5, 0, 0, 0, 0, 6},
			{0, 3, 0, 2, 0, 0, 9, 0, 0}};
			*/
	int f[9][9];
/*	for (int i = 0; i <9; i++){
		cin >> f[0][i]>>f[1][i]>>f[2][i]>>f[3][i]>>f[4][i]
			>>f[5][i]>>f[6][i]>>f[7][i]>>f[8][i];
	}*/
	char buffer[255];
	while (cin.getline(buffer, 255)){
		for (int y = 0; y < 9; y++){
			for (int x = 0; x < 9; x++){
				int index = x + 9*y;
				if (buffer[index] > '0' && buffer[index] <= '9'){
					f[x][y] = buffer[index] - '0';
				} else{
					f[x][y] = 0;
				}
			}
		}

		sudoku a(f);
		a.solve();
		//	a.pretty_print(cout);
/*		bool s = a.solve();
		if (s) {
			cout << "Solved! Hurray.\n";
		} else {
			cout << "Not solved :(\n";
			if (a.is_stuck()){
				cout << "And stuck :((\n";
			} else {
				cout << "But at least not stuck\n";
			}
//			a.pretty_print(cout);
		}
//		a.pretty_print(cout);
//		*/
		a.print(cout);
	}
	return 0;
}
