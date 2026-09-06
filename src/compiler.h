#ifndef compiler_h
#define compiler_h

#include "chunk.h"
#include "common.h"
#include "parser.h"
#include "precedence.h"
#include "rule.h"
#include "scanner.h"
#include "token.h"
#include "value.h"
#include <cstddef>
#include <memory>
#include <string_view>

using RuleTable = std::array<ParseRule, static_cast<size_t>(TOKEN_COUNT)>;

class Compiler {
    std::unique_ptr<Scanner> scanner{};
    std::unique_ptr<Parser> parser{};

    Chunk* compilation_chunk{nullptr};

  public:
    Compiler()
        : scanner(std::make_unique<Scanner>()),
          parser(std::make_unique<Parser>()) {}
    bool compile(std::string_view source, Chunk& chunk);

  private:
    void advance();
    void expression();
    void consume(TokenType type, std::string_view message);
    void end();

    void number();
    void grouping();
    void unary();
    void binary();

    Chunk* current();
    void emit_byte(u8 byte);
    void emit_bytes(u8 first, u8 second);
    void emit_return();
    void emit_const(value val);

    u8 constant(value val);

    void parse_precedence(Precedence precedence);

    void error_at_current(std::string_view message);
    void error(std::string_view message);
    void error_at(Token& token, std::string_view message);

    static constexpr RuleTable make_rules();
    static RuleTable rules;

    const ParseRule& get_rule(TokenType type) const {
        return rules[static_cast<size_t>(type)];
    }
};

#endif
