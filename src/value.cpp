#include "value.h"

void Values::write(value v) { values.push_back(std::move(v)); }
