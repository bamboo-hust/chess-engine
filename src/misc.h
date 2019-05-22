#ifndef MISC_H
#define MISC_H

using namespace std;



struct Point {
    int x, y;

    Point(int x, int y) {
    	this->x = x;
    	this->y = y;
    }

    Point operator + (const Point &o) const {
        return Point(x + o.x, y + o.y);
    }
};

struct Move {
    int from, to;
    Move(int _from, int _to) {
        from = _from;
        to = _to;
    }
};

//using Board = array< array<uint8_t, 8>, 8 >;

#endif // MISC_H