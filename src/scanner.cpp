#include "scanner.h"
#include <string_view>
#include <unordered_map>

static const std::unordered_map<std::string_view, TokenType> KEYWORDS{
    {"and", TokenType::AND},       {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
    {"for", TokenType::FOR},       {"fun", TokenType::FUN},
    {"if", TokenType::IF},         {"nil", TokenType::NIL},
    {"or", TokenType::OR},         {"print", TokenType::PRINT},
    {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
    {"this", TokenType::THIS},     {"true", TokenType::TRUE},
    {"var", TokenType::VAR},       {"while", TokenType::WHILE},
};

void Scanner::init(std::string_view s) {
    start = 0;
    current = 0;
    line = 1;
    source = s;
}

Token Scanner::scan() {
    skip_whitespaces();

    start = current;
    if (finished()) {
        return create(TokenType::LOX_EOF);
    }

    char c = advance();
    if (is_alpha(c))
        return identifier();
    if (is_digit(c))
        return number();

    switch (c) {
    case '(':
        return create(TokenType::LEFT_PAREN);
    case ')':
        return create(TokenType::RIGHT_PAREN);
    case '{':
        return create(TokenType::LEFT_BRACE);
    case '}':
        return create(TokenType::RIGHT_BRACE);
    case ';':
        return create(TokenType::SEMICOLON);
    case ',':
        return create(TokenType::COMMA);
    case '.':
        return create(TokenType::DOT);
    case '-':
        return create(TokenType::MINUS);
    case '+':
        return create(TokenType::PLUS);
    case '/':
        return create(TokenType::SLASH);
    case '*':
        return create(TokenType::STAR);
    case '!':
        return create(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
    case '=':
        return create(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
    case '<':
        return create(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
    case '>':
        return create(match('=') ? TokenType::GREATER_EQUAL
                                 : TokenType::GREATER);
    case '"':
        while (peek() != '"' && !finished()) {
            if (peek() == '\n')
                line++;
            advance();
        }

        if (finished())
            return error("Unterminated string");

        advance();
        return create(TokenType::STRING);
    }

    return error("Unexpected character");
}

void Scanner::skip_whitespaces() {
    while (char c = peek()) {
        switch (c) {
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
        case '\n':
            line++;
            advance();
            break;
        case '/':
            if (peek_next() == '/') {
                while (peek() != '\n' && !finished())
                    advance();
            } else {
                return;
            }
            break;
        default:
            return;
        }
    }
}

bool Scanner::finished() { return current >= source.size(); }

Token Scanner::create(TokenType type) {
    return {.type = type,
            .token = source.substr(start, current - start),
            .line = line};
}

char Scanner::advance() { return source[current++]; }

bool Scanner::is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

Token Scanner::identifier() {
    while (is_alpha(peek()) || is_digit(peek()))
        advance();
    return create(identifier_type());
}

TokenType Scanner::identifier_type() {
    std::string_view lexeme = source.substr(start, current - start);
    auto it = KEYWORDS.find(lexeme);
    if (it == KEYWORDS.end())
        return TokenType::IDENTIFIER;
    else
        return it->second;
}

bool Scanner::is_digit(char c) { return c >= '0' && c <= '9'; }

Token Scanner::number() {
    while (is_digit(peek()))
        advance();

    if (peek() == '.' && is_digit(peek_next())) {
        advance(); // consume .
        while (is_digit(peek()))
            advance();
    }

    return create(TokenType::NUMBER);
}

bool Scanner::match(char check) {
    if (finished())
        return false;

    if (source[current] != check)
        return false;

    return true;
}

Token Scanner::error(std::string_view msg) {
    return {.type = TokenType::ERROR, .token = msg, .line = line};
}

char Scanner::peek() { return source[current]; }

char Scanner::peek_next() {
    if (finished())
        return '\0';
    return source[current + 1];
}
