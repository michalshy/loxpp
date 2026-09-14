#ifndef opcode_h
#define opcode_h

#include "common.h"

enum class OpCode : u8 {
    CONSTANT,
    CONSTANT_LONG,
    NIL,
    TRUE,
    FALSE,
    EQUAL,
    GREATER,
    LESS,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    NOT,
    NEGATE,
    PRINT,
    POP,
    RETURN,
};

#endif
