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
	cout << "Yasss " << VERSION_STRING << "\n"
	<< "Usage:\n"
	<< program_name << " [-vcasC]\n"
	<< program_name << " --help\n"
	<< program_name << " --version\n"
	<< program_name << " --generate[=num]\n"
	<< "\nOptions:\n"
	<< "\t-a|--answer\tPrints out the solved Sudoku\n"
	<< "\t-c|--count\tPrints the number of soltuions to a given Sudoku\n"
	<< "\t-C|--canonical\tTransforms to a canonical form\n"
	<< "\t-g|--generate[=num]\tGenerates num (default 1) Sudoku\n"
	<< "\t-m|--minimalise\tMinimalize the given Sduoku\n"
	<< "\t-s|--score\tPrints out a difficulty rating (score)\n"
	<< "\t-v|--verbose\tCurrently ignored\n"
	<< "\n"
	"Yasss reads Sudokus from STDIN (one per line) and prints out the solution\n"
	<< "unless one of the Options -c or -s is given.\n"
	<< "Option --answer|-a forces the solved Sudoku to be printed.\n"
	<< "If option --generate|-g is given, all other options are ignored.\n"
	<< "if option --canonical is given it is applied before all other Options\n"
	;


}

void generate(int count){
	for (int i = 0; i < count; i++){
		sudoku a;
		a.random_generate(27);
		a.print(cout);
	}
}

void generate_17(void){
	sudoku a;
	a.generate_17();
}

int main(int argc, char** argv){

	// parse command line options:
	static struct option long_options[] = {
		{"help", no_argument, 0, 0},
		{"verbose", no_argument, 0, 1},
		{"version", no_argument, 0, 2},
		{"count", no_argument, 0, 3},
		{"score", no_argument, 0, 4},
		{"answer", no_argument, 0, 5},
		{"generate", optional_argument, 0, 6},
		{"canonical", no_argument, 0, 7},
		{"17", no_argument, 0, 8},
		{"minimalise", no_argument, 0, 9},
	};
	int option_result = 0;
	int option_index = 0;
	bool verbose = false;
	bool print_solution_count = false;
	bool print_score = false;
	bool print_solution = false;
	bool print_canonical = false;
	bool minimalise = false;
	while (true){
		option_result = getopt_long(argc, argv, "hvVcsamg::C", 
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
				break;
			case 'c':
			case 3: 
				print_solution_count = true;
				break;
			case 's':
			case 4:
				print_score = true;
				break;
			case 'a':
			case '5':
				print_solution = true;
				break;
			case 'C':
			case 7:
				print_canonical = true;
				break;
			case 8:
				generate_17();
				exit(0);
				break;
			case 'm':
			case '9':
				minimalise = true;
				break;
			case 'g':
			case 6:
				int count = 1;
				if (optarg){
					count = atoi(optarg);
				} else if (optind < argc){
					count = atoi(argv[optind]);
				}
				generate(count);
				exit(0);
				break;
		}

	}

	if (optind < argc){
		cerr << argv[0] << ": Warning: ignoring additional command line arguments\n";
	}


	int f[9][9];
	char buffer[255];
	while (cin.getline(buffer, 255)){
		bool msg = true;
		for (int y = 0; y < 9; y++){
			for (int x = 0; x < 9; x++){
				int index = x + 9*y;
				if (buffer[index] == '\0' && msg){
					cerr << "Premature end of input, padding with zeros...\n";
					msg = false;
				}
				int val = buffer[index] - '0';
				if (val > 0 && val <= 9 && msg){
					f[x][y] = val;
				} else{
					f[x][y] = 0;
				}
			}
		}

		sudoku a(f);
		if (print_canonical){
			a.to_canonical_form();
			a.print(cout);
		}

		if (minimalise) {
			a.minimalise();
			a.print(cout);
		}

		a.count_solutions = print_solution_count;
		a.calculate_difficulty_rating = print_score;
		a.solve();
		if (print_solution_count){
			cout << "Number of solutions: " 
				<< a.get_solution_count() << "\n";
		} 			
		if (print_score){
			cout << "Score: " << a.get_difficulty_rating() 
				<< "\n";
		}
		
		if (print_solution || (!print_score && !print_solution_count
					&& !print_canonical && !minimalise)){
			a.print(cout);
		}

	}
	return 0;
}

