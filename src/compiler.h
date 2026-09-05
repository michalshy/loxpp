#ifndef compiler_h
#define compiler_h

#include "chunk.h"
#include "common.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "value.h"
#include <cstddef>
#include <memory>
#include <string_view>

class Compiler {
    std::unique_ptr<Scanner> scanner{};
    std::unique_ptr<Parser> parser{};

    Chunk* compilation_chunk{nullptr};

  public:
    Compiler() : scanner(std::make_unique<Scanner>()) {}
    bool compile(std::string_view source, Chunk& chunk);

  private:
    void advance();
    void expression();
    void consume(TokenType type, std::string_view message);
    void end();

    void number();
    void grouping();
    void unary();

    Chunk* current();
    void emit_byte(u8 byte);
    void emit_bytes(u8 first, u8 second);
    void emit_return();
    void emit_const(value val);

    u8 constant(value val);

    void error_at_current(std::string_view message);
    void error(std::string_view message);
    void error_at(Token& token, std::string_view message);
};

#endif
