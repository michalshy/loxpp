#ifndef scanner_h
#define scanner_h

#include "common.h"
#include "token.h"
#include <string_view>

class Scanner {
    std::string_view source;
    u64 start{0};
    u64 current{0};
    u64 line{1};

  public:
    void init(std::string_view);
    [[nodiscard]] Token scan();

  private:
    void skip_whitespaces();
    [[nodiscard]] bool finished();
    [[nodiscard]] Token create(TokenType);
    char advance();
    [[nodiscard]] bool is_alpha(char);
    [[nodiscard]] Token identifier();
    [[nodiscard]] TokenType identifier_type();
    [[nodiscard]] bool is_digit(char);
    [[nodiscard]] Token number();
    [[nodiscard]] bool match(char);
    [[nodiscard]] Token error(std::string_view);
    [[nodiscard]] char peek();
    [[nodiscard]] char peek_next();
};

#endif
