#ifndef compiler_h
#define compiler_h

#include "scanner.h"
#include <memory>
#include <string_view>

class Compiler {
    std::unique_ptr<Scanner> scanner{};

  public:
    Compiler() : scanner(std::make_unique<Scanner>()) {}
    void compile(std::string_view source);
};

#endif
