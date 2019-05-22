#ifndef UTILS_H
#define UTILS_H

#include "misc.h"
#include "constants.h"

namespace Utils {
	bool is_uppercase(char c) {
		return 'A' <= c && c <= 'Z';
	}

	bool is_lowercase(char c) {
		return 'a' <= c && c <= 'z';
	}

	char swapcase(char c) {
		if (is_uppercase(c)) return (c + ('a' - 'A'));
		if (is_lowercase(c)) return (c - ('a' - 'A'));
		return c;
	}

	string swapcase(const string &s) {
		string t;
		t.reserve(s.size());
		for (char c : s) t += swapcase(c);
		return t;
	}

	string reverse(const string &s) {
		string t;
		t.reserve(s.size());
		for (int i = (int)s.size() - 1; i >= 0; --i) t += s[i];
		return t;
	}
}

#endif // UTILS_H