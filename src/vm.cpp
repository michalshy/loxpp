#include "vm.h"
#include "opcode.h"
#include <expected>

std::expected<void, InterpretError> VM::interpret(Chunk& chunk) {
    curr_chunk = &chunk;
    ip = 0;
    return run();
}

std::expected<void, InterpretError> VM::run() {
    bool exit{false};
    if (!curr_chunk) {
        return std::unexpected(InterpretError::RUNTIME_ERROR);
    }

    while (!exit) {
        OpCode instruction = static_cast<OpCode>(curr_chunk->byte(ip++));
        switch (instruction) {
        case OpCode::RETURN:
            return {};
        default:
            break;
        }
    }

    return std::unexpected(InterpretError::RUNTIME_ERROR);
}
