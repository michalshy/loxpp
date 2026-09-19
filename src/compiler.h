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
    void declaration();
    void statement();
    void synchronize();
    void expression();
    void consume(TokenType type, std::string_view message);
    void end();

    void var_declaration();
    u8 parse_var(std::string_view error_msg);
    u8 id_constant(Token token);
    void define_var(u8 var);

    void print();
    void expression_internal();

    void number(bool);
    void grouping(bool);
    void unary(bool);
    void binary(bool);
    void literal(bool);
    void string(bool);
    void variable(bool);

    void named_variable(Token token, bool can_assign);

    Chunk* current();
    void emit_byte(u8 byte);
    void emit_bytes(u8 first, u8 second);
    void emit_return();
    void emit_const(value val);

    u8 constant(value val);

    void parse_precedence(Precedence precedence);

    bool match(TokenType type);
    bool check(TokenType type);

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
