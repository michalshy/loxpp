#ifndef vm_h
#define vm_h

#include "chunk.h"
#include "value.h"
#include <expected>
#include <stack>

enum class InterpretError {
    COMPILE_ERROR,
    RUNTIME_ERROR,
};

class VM {
    Chunk* curr_chunk{nullptr};
    size_t ip{0};

    std::stack<value> stack;

  public:
    static VM& Instance() {
        static VM v;
        return v;
    }

    std::expected<void, InterpretError> interpret(Chunk& chunk);

  private:
    std::expected<void, InterpretError> run();

    value pop_stack();
    VM() = default;
};

namespace detail {
size_t constant_idx(Chunk&, size_t, OpCode);
}

#endif
