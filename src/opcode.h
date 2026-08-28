#ifndef opcode_h
#define opcode_h

#include "common.h"

enum class OpCode : u8 {
    CONSTANT,
    CONSTANT_LONG,
    RETURN,
};

#endif
