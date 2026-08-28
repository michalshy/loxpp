#ifndef vm_h
#define vm_h

#include "chunk.h"
#include <expected>

enum class InterpretError {
    COMPILE_ERROR,
    RUNTIME_ERROR,
};

class VM {
    Chunk* curr_chunk{nullptr};
    size_t ip{0};

  public:
    static VM& Instance() {
        static VM v;
        return v;
    }

    std::expected<void, InterpretError> interpret(Chunk& chunk);

  private:
    std::expected<void, InterpretError> run();
    VM() = default;
};

#endif
