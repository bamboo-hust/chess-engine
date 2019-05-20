#ifndef POSITION_H
#define POSITION_H

#include <bits/stdc++.h>

using namespace std;


struct Position {
	Board board;
	int score;
	bool wc[2];
	bool bc[2];
	
	vector<Point> gen_moves() {
		for (int x = 0; x < 8; ++x) {
			for (int y = 0; y < 8; ++y) {
				if (true) {
					for (Point d : DIRECTIONS[board[x][y]]) {

					}
				}
			}
		}

		return vector<Point>();
	}
};



#endif // POSITION_H