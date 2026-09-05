#ifndef parser_h
#define parser_h

#include "token.h"

struct Parser {
    Token curr{};
    Token prev{};
    bool had_error{false};
    bool panic{false};
};

#endif
