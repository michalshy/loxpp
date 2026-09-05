#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "opcode.h"
#include <expected>
#include <print>
#include <string_view>

std::expected<void, InterpretError> VM::interpret(std::string_view source) {
    Chunk chunk{};

    if (compiler->compile(source, chunk)) {
        return std::unexpected(InterpretError::COMPILE_ERROR);
    }

    curr_chunk = &chunk;
    ip = 0;

    return run();
}

std::expected<void, InterpretError> VM::interpret(Chunk& chunk) {
    curr_chunk = &chunk;
    ip = 0;
    std::println("interpreting...");
    return run();
}

std::expected<void, InterpretError> VM::run() {
    bool exit{false};
    if (!curr_chunk) {
        return std::unexpected(InterpretError::RUNTIME_ERROR);
    }

    while (!exit) {
#if DEBUG
        debug::disassemble(*curr_chunk, ip);
#endif

        OpCode instruction = static_cast<OpCode>(curr_chunk->byte(ip++));
        switch (instruction) {
        case OpCode::RETURN:
            std::println("{}", pop_stack());
            return {};
        case OpCode::CONSTANT: {
            size_t idx = detail::constant_idx(*curr_chunk, ip, instruction);
            stack.push(curr_chunk->constant(idx));
            ip++;
            break;
        }
        case OpCode::CONSTANT_LONG: {
            size_t idx = detail::constant_idx(*curr_chunk, ip, instruction);
            stack.push(curr_chunk->constant(idx));
            ip += 3;
            break;
        }
        case OpCode::ADD:
            binary(std::plus<>{});
            break;
        case OpCode::SUBTRACT:
            binary(std::minus<>{});
            break;
        case OpCode::MULTIPLY:
            binary(std::multiplies<>{});
            break;
        case OpCode::DIVIDE:
            binary(std::divides<>{});
            break;
        case OpCode::NEGATE: {
            stack.top() = stack.top() * -1;
            break;
        }
        default:
            break;
        }
    }

    return std::unexpected(InterpretError::RUNTIME_ERROR);
}

value VM::pop_stack() {
    value v = stack.top();
    stack.pop();
    return v;
}

namespace detail {
size_t constant_idx(Chunk& chunk, size_t offset, OpCode opcode) {
    if (opcode == OpCode::CONSTANT) {
        return static_cast<size_t>(chunk.byte(offset));
    } else {
        return static_cast<size_t>(chunk.byte(offset + 2) << 16 |
                                   chunk.byte(offset + 1) << 8 |
                                   chunk.byte(offset));
    }
}
} // namespace detail
