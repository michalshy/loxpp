#ifndef src_value_h
#define src_value_h

#include "common.h"
#include <vector>

using value = double;

class Values
{
    std::vector<value> values{};

public:
    void write(value v);

    [[nodiscard]] const value& get(u8 idx) { return values[idx]; }
    [[nodiscard]] u64 count() const { return values.size(); }
    [[nodiscard]] u64 capacity() const { return values.capacity(); }
};

#endif // src_value_h