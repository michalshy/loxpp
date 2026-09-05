#include "compiler.h"
#include "opcode.h"
#include "token.h"
#include "value.h"
#include <print>
#include <string>
#include <string_view>
#include <utility>

bool Compiler::compile(std::string_view source, Chunk& chunk) {
    scanner->init(source);
    compilation_chunk = &chunk;

    parser->had_error = false;
    parser->panic = false;

    advance();
    expression();
    consume(TokenType::LOX_EOF, "Expected end of expression.");
    end();
    return !parser->had_error;
}

void Compiler::advance() {
    bool exit{false};
    parser->prev = parser->prev;

    while (!exit) {
        parser->curr = scanner->scan();
        if (parser->curr.type != TokenType::ERROR)
            exit = true;

        error_at_current(parser->curr.token);
    }
}

void Compiler::expression() {}

void Compiler::consume(TokenType type, std::string_view message) {
    if (parser->curr.type == type) {
        advance();
    } else {
        error_at_current(message);
    }
}

void Compiler::end() { emit_return(); }

void Compiler::number() {
    value v = std::stod(parser->prev.token.data());
    emit_const(v);
}

void Compiler::grouping() {
    expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
}

void Compiler::unary() {}

Chunk* Compiler::current() { return compilation_chunk; }

void Compiler::emit_byte(u8 byte) {
    Chunk* c = current();
    c->write(byte, parser->prev.line);
}

void Compiler::emit_bytes(u8 first, u8 second) {
    emit_byte(first);
    emit_byte(second);
}

void Compiler::emit_return() { emit_byte(std::to_underlying(OpCode::RETURN)); }

void Compiler::emit_const(value val) {
    emit_bytes(std::to_underlying(OpCode::RETURN), constant(val));
}

u8 Compiler::constant(value v) {
    Chunk* curr = current();
    size_t idx = curr->add_constant(v);
    if (idx > UINT8_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }

    return static_cast<u8>(idx);
}

void Compiler::error_at_current(std::string_view message) {
    error_at(parser->curr, message);
}

void Compiler::error(std::string_view message) {
    error_at(parser->prev, message);
}

void Compiler::error_at(Token& token, std::string_view message) {
    if (parser->panic)
        return;
    else
        parser->panic = true;

    std::print("[line {}] Error", token.line);

    if (token.type == TokenType::LOX_EOF) {
        std::print(" at the end");
    } else if (token.type == TokenType::ERROR) {
        // nothing
    } else {
        std::print(" at '{}'", token.token);
    }

    std::println(": {}", message);
    parser->had_error = true;
}
