#ifndef src_line_h
#define src_line_h

#include "common.h"
class Line
{
    u64 count{1};
    u64 line;

public:
    explicit Line(u64 l) : line(l){}

    void push() { count++; }
    void pull() { count--; }

    u64 get_line() { return line; }
    u64 get_count() { return count; }
};

#endif // src_line_h