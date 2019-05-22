#ifndef POSITION_H
#define POSITION_H

#include <vector>
#include "utils.h"

struct Position {
	string board;
	int score;
	array<bool, 2> wc;
	array<bool, 2> bc;
	int ep;
	int kp;
	
	vector<Move> gen_moves() {
		vector<Move> res;
		for (int i = 0; i < board.size(); ++i) {
			char p = board[i];
			if (!Utils::is_uppercase(p)) continue;
			for (int d : directions[p]) {
				for (int j = i + d; ; j += d) {
					char q = board[j];
					if (q == ' ' || Utils::is_uppercase(q)) break;
					if (p == 'P') {
						if ((d == N || d == N + N) && q != '.') break;
						if (d == N + N && (i < A1 + N || board[i + N] != '.')) break;
						if ((d == (N + W) || d == (N + E)) && q == '.' && (j != ep && j != kp)) break;
					}
					res.push_back(Move(i, j));
					if (p == 'P' || p == 'N' || p == 'K' || !Utils::is_uppercase(q)) break;
					if (i == A1 && board[j + E] == 'K' && wc[0]) res.push_back(Move(j + E, j + W));
					if (i == H1 && board[j + W] == 'K' && wc[1]) res.push_back(Move(j + W, j + E));
				}
			}
		}
		return res;
	}

	Position rotate() {
		return Position{Utils::swapcase(board), -score, bc, wc, ep ? 199 - ep : 0, kp ? 199 - kp : 0};
	}

	Position nullmove() {
		return Position{Utils::swapcase(board), -score, bc, wc, 0, 0};	
	}

	Position move(Move _move) {
		int i = _move.from;
		int j = _move.to;
		char p = board[i];
		char q = board[j];
		Position res = *this;
		res.score += value(_move);
		res.board[j] = board[i];
		res.board[i] = '.';

		if (i == A1) res.wc[0] = 0;
		if (i == H1) res.wc[1] = 0;
		if (j == A8) res.bc[1] = 0;
		if (j == H8) res.bc[0] = 0;

		if (p == 'K') {
			res.wc[0] = res.wc[1] = 0;
			if (abs(j - i) == 2) {
				res.kp = (i + j) / 2;
				res.board[j < i ? A1 : H1] = '.';
				res.board[kp] = 'R';
			}
		}

		if (p == 'P') {
			if (A8 <= j && j <= H8) {
				res.board[j] = 'Q';
			}
			if (j - i == 2 * N) {
				res.ep = i + N;
			}
			if ((j - i == N + W || j - i == N + E) && q == '.') {
				res.board[j + S] = '.';
			}
		}

		return res.rotate();
	}

	int value(Move _move) {
		int i = _move.from;
		int j = _move.to;
		char p = board[i];
		char q = board[j];

		int movescore = pst[p][j] - pst[p][i]; // positional difference

		// capture
		if (!Utils::is_uppercase(q)) movescore += pst[Utils::swapcase(q)][119 - j];
		// castling check
		if (abs(j - kp) < 2) movescore += pst['K'][119 - j];
		// castling
		if (p == 'K' && abs(i - j) == 2) {
			movescore += pst['R'][(i + j) / 2];
			movescore -= pst['R'][j < i ? A1 : H1];
		}
		if (p == 'P') {
			if (A8 <= j && j <= H8) movescore += pst['Q'][j] - pst['P'][j];
			if (j == ep) score += pst['P'][119-(j+S)];
		}

		return movescore;
	}
};



#endif // POSITION_H