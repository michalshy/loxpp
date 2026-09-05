#ifndef rule_h
#define rule_h

#include "precedence.h"

class Compiler;

using ParseFn = void (Compiler::*)();

struct ParseRule {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
};

#endif
