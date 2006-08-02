#define VERSION_STRING "0.4.4"
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

extern char* optarg;
extern int optind, opterr, optopt;
#include <getopt.h>


using namespace std;

void help(char* program_name) {
	cout << "Yasss " << VERSION_STRING << "\n";
	cout << "Usage:\n";
	cout << program_name << " [--verbose|-v] [--count|-c]\n";
	cout << program_name << " --help\n";
	cout << program_name << " --version\n";
	cout << "Options:\n";
	cout << "\t--verbose|-v\tCurrently ignored\n";
	cout << "\t--count|-c\tPrints out how many solutions a given Sudoku has\n";
	cout << "\n"
	<< "Yasss reads Sudokus from STDIN (one per line) and prints out the solution\nunless"
	<< " option --count is given, it which case it prints the number of \n"
	<< "distinct solutions\n";


}


int main(int argc, char** argv){

	// parse command line options:
	static struct option long_options[] = {
		{"help", no_argument, 0, 0},
		{"verbose", no_argument, 0, 1},
		{"version", no_argument, 0, 2},
		{"count", no_argument, 0, 3}};
	int option_result = 0;
	int option_index = 0;
	bool verbose = false;
	bool print_solution_count = false;
	while (true){
		option_result = getopt_long(argc, argv, "hvVc", 
				long_options, &option_index);
		if (option_result == -1){
			break;
		}
		switch (option_result){
			case 'h':
			case 0:
				help(argv[0]);
				exit(0);
			case 'v':
			case 1:
				verbose = true;
				break;
			case 'V':
			case 2:
				cout << "yasss " << VERSION_STRING << "\n";
			case 'c':
			case 3: 
				print_solution_count = true;
		}

	}


	int f[9][9];
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
		if (print_solution_count){
			a.test_if_uniq = true;
			a.solve();
			cout << a.get_solution_count() << "\n";
		} else {
			a.solve();
			a.print(cout);
		}
	}
	return 0;
}

