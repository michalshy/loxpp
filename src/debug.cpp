#include "debug.h"
#include "chunk.h"
#include "common.h"
#include "opcode.h"
#include "value.h"
#include "vm.h"
#include <print>
#include <string_view>

namespace debug {

namespace helper {
bool line_same_as_previous(Chunk& chunk, size_t offset) {
    if (offset > 0 && chunk.get_line(offset) == chunk.get_line(offset - 1))
        return true;
    return false;
}
} // namespace helper

namespace internal {
size_t simple_instr(std::string_view name, size_t offset) {
    std::println("      {}", name);
    return offset + 1;
}

size_t constant_instr(std::string_view name, Chunk& chunk, size_t offset) {
    size_t idx = detail::constant_idx(chunk, offset + 1);

    std::print("    {} {:4d}", name, idx); // chunk header

    value val = chunk.constant(idx);
    PrintVal(val);
    std::println();

    u8 additional = 2;
    return offset + additional;
}
} // namespace internal

void disassemble(Chunk& chunk, std::string_view name) {
    std::println("== {} ==", name);

    for (size_t offset = 0; offset < chunk.size();) {
        offset = disassemble(chunk, offset);
    }
}

u64 disassemble(Chunk& chunk, size_t offset) {
    std::print("{:04d}", offset);

    if (helper::line_same_as_previous(chunk, offset)) {
        std::print("  | ");
    } else {
        std::print("{:4d}", chunk.get_line(offset));
    }

    u8 raw = chunk.byte(offset);
    OpCode instruction = static_cast<OpCode>(raw);
    switch (instruction) {
    case OpCode::RETURN:
        return internal::simple_instr("RETURN", offset);
    case OpCode::CONSTANT:
        return internal::constant_instr("CONSTANT", chunk, offset);
    case OpCode::FALSE:
        return internal::simple_instr("FALSE", offset);
        break;
    case OpCode::TRUE:
        return internal::simple_instr("TRUE", offset);
        break;
    case OpCode::NIL:
        return internal::simple_instr("NIL", offset);
        break;
    case OpCode::NEGATE:
        return internal::simple_instr("NEGATE", offset);
    case OpCode::ADD:
        return internal::simple_instr("ADD", offset);
    case OpCode::SUBTRACT:
        return internal::simple_instr("SUBTRACT", offset);
    case OpCode::MULTIPLY:
        return internal::simple_instr("MULTIPLY", offset);
    case OpCode::DIVIDE:
        return internal::simple_instr("DIVIDE", offset);
    case OpCode::NOT:
        return internal::simple_instr("NOT", offset);
    case OpCode::EQUAL:
        return internal::simple_instr("EQUAL", offset);
    case OpCode::LESS:
        return internal::simple_instr("LESS", offset);
    case OpCode::GREATER:
        return internal::simple_instr("GREATER", offset);
    case OpCode::PRINT:
        return internal::simple_instr("PRINT", offset);
    case OpCode::POP:
        return internal::simple_instr("POP", offset);
    case OpCode::DEFINE_GLOBAL:
        return internal::constant_instr("DEFINE_GLOBAL", chunk, offset);
    case OpCode::GET_GLOBAL:
        return internal::constant_instr("GET_GLOBAL", chunk, offset);

    default:
        std::println("Unknown opcode {:d}", raw);
        return offset + 1;
    }
}

} // namespace debug
