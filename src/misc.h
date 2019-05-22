#ifndef MISC_H
#define MISC_H

#include <map>

#define fast_map std::map

struct Move {
    int from, to;
    Move(int _from, int _to) {
        from = _from;
        to = _to;
    }
};

#endif // MISC_H