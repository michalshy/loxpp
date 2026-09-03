#ifndef compiler_h
#define compiler_h

#include "chunk.h"
#include "scanner.h"
#include <memory>
#include <string_view>

class Compiler {
    std::unique_ptr<Scanner> scanner{};

  public:
    Compiler() : scanner(std::make_unique<Scanner>()) {}
    bool compile(std::string_view source, Chunk& chunk);
};

#endif
