#ifndef opcode_h
#define opcode_h

#include "common.h"

enum class OpCode : u8 {
    CONSTANT,
    CONSTANT_LONG,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    NEGATE,
    RETURN,
};

#endif
