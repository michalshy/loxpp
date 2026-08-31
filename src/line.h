#ifndef line_h
#define line_h

#include "common.h"
class Line {
    u64 line{};
    u64 offset{};

  public:
    Line(u64 l, u64 off) : line(l), offset(off) {}

    [[nodiscard]] u64 get_line() const { return line; }
    [[nodiscard]] u64 get_offset() const { return offset; }
};

#endif // line_h
