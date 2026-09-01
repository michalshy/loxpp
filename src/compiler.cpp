#include "compiler.h"
#include "token.h"
#include <print>

void Compiler::compile(std::string_view source) {
    bool exit{false};
    scanner->init(source);
    u64 line{0};
    while (!exit) {
        Token token = scanner->scan();
        if (token.line != line) {
            std::print("{:4d} ", token.line);
            line = token.line;
        } else {
            std::print("   | ");
        }

        std::println("{} '{}'", token.type, token.token);

        if (token.type == TokenType::LOX_EOF)
            exit = true;
    }
}
