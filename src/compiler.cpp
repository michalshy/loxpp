#include "compiler.h"
#include "debug.h"
#include "opcode.h"
#include "precedence.h"
#include "rule.h"
#include "token.h"
#include "value.h"
#include <cstddef>
#include <print>
#include <string>
#include <string_view>
#include <utility>

RuleTable Compiler::rules = Compiler::make_rules();

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

void Compiler::expression() { parse_precedence(Precedence::ASSIGNMENT); }

void Compiler::consume(TokenType type, std::string_view message) {
    if (parser->curr.type == type) {
        advance();
    } else {
        error_at_current(message);
    }
}

void Compiler::end() {
#if DEBUG_PRINT_CODE
    if (!parser->had_error) {
        debug::disassemble(*current(), "code");
    }
#endif
    emit_return();
}

void Compiler::number() {
    value v = std::stod(parser->prev.token.data());
    emit_const(v);
}

void Compiler::grouping() {
    expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
}

void Compiler::unary() {
    TokenType op = parser->prev.type;
    parse_precedence(Precedence::UNARY);
    if (op == TokenType::MINUS) {
        emit_byte(std::to_underlying(OpCode::NEGATE));
    }
}

void Compiler::binary() {
    TokenType op_type = parser->prev.type;
    const ParseRule& rule = get_rule(op_type);

    parse_precedence(
        static_cast<Precedence>(static_cast<u8>(rule.precedence) + 1));

    if (op_type == TokenType::MINUS) {
        emit_byte(std::to_underlying(OpCode::SUBTRACT));
    } else if (op_type == TokenType::PLUS) {
        emit_byte(std::to_underlying(OpCode::ADD));
    } else if (op_type == TokenType::STAR) {
        emit_byte(std::to_underlying(OpCode::MULTIPLY));
    } else if (op_type == TokenType::SLASH) {
        emit_byte(std::to_underlying(OpCode::DIVIDE));
    }
}

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

void Compiler::parse_precedence(Precedence precedence) {
    advance();
    ParseFn prefix_rule = get_rule(parser->prev.type).prefix;
    if (prefix_rule == nullptr) {
        error("Expected expression.");
    } else {
        (this->*prefix_rule)();
    }

    while (precedence <= get_rule(parser->curr.type).precedence) {
        advance();
        ParseFn infix_rule = get_rule(parser->prev.type).infix;
        (this->*infix_rule)();
    }
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

constexpr std::array<ParseRule, static_cast<size_t>(TOKEN_COUNT)>
Compiler::make_rules() {
    std::array<ParseRule, static_cast<size_t>(TOKEN_COUNT)> r{};

    auto set = [&r](TokenType t, ParseFn prefix, ParseFn infix,
                    Precedence prec) {
        r[static_cast<size_t>(t)] = {prefix, infix, prec};
    };

    set(TokenType::LEFT_PAREN, &Compiler::grouping, nullptr, Precedence::NONE);
    set(TokenType::RIGHT_PAREN, nullptr, nullptr, Precedence::NONE);
    set(TokenType::LEFT_BRACE, nullptr, nullptr, Precedence::NONE);
    set(TokenType::RIGHT_BRACE, nullptr, nullptr, Precedence::NONE);
    set(TokenType::COMMA, nullptr, nullptr, Precedence::NONE);
    set(TokenType::DOT, nullptr, nullptr, Precedence::CALL);
    set(TokenType::MINUS, &Compiler::unary, &Compiler::binary,
        Precedence::TERM);
    set(TokenType::PLUS, nullptr, &Compiler::binary, Precedence::TERM);
    set(TokenType::SEMICOLON, nullptr, nullptr, Precedence::NONE);
    set(TokenType::SLASH, nullptr, &Compiler::binary, Precedence::FACTOR);
    set(TokenType::STAR, nullptr, &Compiler::binary, Precedence::FACTOR);
    set(TokenType::BANG, &Compiler::unary, nullptr, Precedence::NONE);
    set(TokenType::BANG_EQUAL, nullptr, &Compiler::binary,
        Precedence::EQUALITY);
    set(TokenType::EQUAL, nullptr, nullptr, Precedence::NONE);
    set(TokenType::EQUAL_EQUAL, nullptr, &Compiler::binary,
        Precedence::EQUALITY);
    set(TokenType::GREATER, nullptr, &Compiler::binary, Precedence::COMPARISON);
    set(TokenType::GREATER_EQUAL, nullptr, &Compiler::binary,
        Precedence::COMPARISON);
    set(TokenType::LESS, nullptr, &Compiler::binary, Precedence::COMPARISON);
    set(TokenType::LESS_EQUAL, nullptr, &Compiler::binary,
        Precedence::COMPARISON);
    set(TokenType::IDENTIFIER, nullptr, nullptr, Precedence::NONE);
    set(TokenType::STRING, nullptr, nullptr, Precedence::NONE);
    set(TokenType::NUMBER, &Compiler::number, nullptr, Precedence::NONE);
    set(TokenType::AND, nullptr, nullptr, Precedence::AND);
    set(TokenType::CLASS, nullptr, nullptr, Precedence::NONE);
    set(TokenType::ELSE, nullptr, nullptr, Precedence::NONE);
    set(TokenType::FALSE, nullptr, nullptr, Precedence::NONE);
    set(TokenType::FOR, nullptr, nullptr, Precedence::NONE);
    set(TokenType::FUN, nullptr, nullptr, Precedence::NONE);
    set(TokenType::IF, nullptr, nullptr, Precedence::NONE);
    set(TokenType::NIL, nullptr, nullptr, Precedence::NONE);
    set(TokenType::OR, nullptr, nullptr, Precedence::OR);
    set(TokenType::PRINT, nullptr, nullptr, Precedence::NONE);
    set(TokenType::RETURN, nullptr, nullptr, Precedence::NONE);
    set(TokenType::SUPER, nullptr, nullptr, Precedence::NONE);
    set(TokenType::THIS, nullptr, nullptr, Precedence::NONE);
    set(TokenType::TRUE, nullptr, nullptr, Precedence::NONE);
    set(TokenType::VAR, nullptr, nullptr, Precedence::NONE);
    set(TokenType::WHILE, nullptr, nullptr, Precedence::NONE);
    set(TokenType::ERROR, nullptr, nullptr, Precedence::NONE);
    set(TokenType::LOX_EOF, nullptr, nullptr, Precedence::NONE);

    return r;
}
