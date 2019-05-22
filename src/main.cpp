#include <bits/stdc++.h>

using namespace std;

#include "constants.h"
#include "position.h"

int main() {
	init_constants();
	
	Position pos{initial};
	cout << pos.board << endl;
	pos = pos.move(Move(81, 71));
	cout << pos.board << endl;
	return 0;
}