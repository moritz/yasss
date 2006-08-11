#ifndef _MORITZ_SUDOKU_NOSANE_
#define _MORITZ_SUODKU_NOSANE_

#include "sudoku.hpp"

class sudoku_nosane {

	public: 
		sudoku_nosane();
		sudoku sane;
		sudoku solved;
		int data[9][9];
		bool locked[9][9];
		int count();
		bool is_solved();
	protected:
		void null_init();
};

#endif
