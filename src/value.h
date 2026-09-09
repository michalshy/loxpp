#ifndef value_h
#define value_h

#include "common.h"
#include "object.h"
#include <print>
#include <variant>
#include <vector>

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

using value = std::variant<std::monostate, bool, double, Object*>;

static inline void PrintVal(const value& v) {
    std::visit(
        overloaded{[](std::monostate) { std::print("nil"); },
                   [](bool b) { std::print("{}", b ? "true" : "false"); },
                   [](double d) { std::print("{}", d); },
                   [](Object* obj) { obj->print(); }},
        v);
}

class Values {
    std::vector<value> values{};

  public:
    void write(value v);

    [[nodiscard]] const value& get(size_t idx) { return values[idx]; }
    [[nodiscard]] u64 count() const { return values.size(); }
    [[nodiscard]] u64 capacity() const { return values.capacity(); }
};

#endif // value_h
