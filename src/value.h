#ifndef src_value_h
#define src_value_h

#include "common.h"
#include <vector>

using value = double;

class Values
{
    u64 capacity{0};
    u64 count{0};
    std::vector<value> values{};

public:
    void write(value v);
};

#endif // src_value_h