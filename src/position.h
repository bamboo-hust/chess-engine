#ifndef POSITION_H
#define POSITION_H

#include <vector>

struct Position {
	string board;
	int score;
	bool wc[2];
	bool bc[2];
	int ep;
	int kp;
	
	vector<Move> gen_moves() {
		return vector<Move>();
	}
};



#endif // POSITION_H