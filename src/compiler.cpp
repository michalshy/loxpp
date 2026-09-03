#include "compiler.h"
#include "token.h"
#include <print>

bool Compiler::compile(std::string_view source, Chunk& chunk) {
    bool exit{false};
    scanner->init(source);
    advance();
    expression();
    consume(TokenType::LOX_EOF, "Expected end of expression.");
}
