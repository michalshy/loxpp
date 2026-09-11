#ifndef vm_h
#define vm_h

#include "chunk.h"
#include "compiler.h"
#include "opcode.h"
#include "value.h"
#include <expected>
#include <format>
#include <memory>
#include <print>
#include <stack>
#include <string_view>

enum class InterpretError {
    COMPILE_ERROR,
    RUNTIME_ERROR,
};

class VM {
    Chunk* curr_chunk{nullptr};
    size_t ip{0};

    std::stack<value> stack{};

    std::unique_ptr<Compiler> compiler{};

  public:
    static VM& Instance() {
        static VM v;
        return v;
    }

    std::expected<void, InterpretError> interpret(std::string_view source);

  private:
    VM() : compiler(std::make_unique<Compiler>()) {}

    std::expected<void, InterpretError> interpret(Chunk& chunk);
    std::expected<void, InterpretError> run();

    value pop_stack();

    template <typename... Args>
    void runtime_error(std::format_string<Args...> fmt, Args&&... args) {
        std::string message = std::format(fmt, std::forward<Args>(args)...);
        i64 line = curr_chunk->get_line(ip - 1);
        std::print("{} [line {}]", message, line);
        stack = {};
    }

    void add();
    void subtract();
    void divide();
    void multiply();
};

namespace detail {
size_t constant_idx(Chunk&, size_t, OpCode);
} // namespace detail

#endif
