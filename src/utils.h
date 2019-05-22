#ifndef UTILS_H
#define UTILS_H

#include "misc.h"
#include "constants.h"

namespace Utils {
	bool is_uppercase(char c) {
		return 'A' <= c && c <= 'Z';
	}

	char swapcase(char c) {
		return is_uppercase(c) ? c - ('a' - 'A') : c + ('a' - 'A');
	}

	string swapcase(const string &s) {
		string t;
		t.reserve(s.size());
		for (char c : s) t += swapcase(c);
		return t;
	}
}

#endif // UTILS_H