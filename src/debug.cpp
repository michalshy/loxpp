#include "debug.h"
#include "chunk.h"
#include "common.h"
#include <print>
#include <string_view>

namespace debug
{

namespace internal
{
    static size_t simple_instr(std::string_view name, size_t offset)
    {
        std::println("{}", name);
        return offset + 1;
    }
}

void disassemble(Chunk& chunk, std::string_view name)
{
    std::println("== {} ==", name);

    for (size_t offset = 0; offset < chunk.size();)
    {
        offset = disassemble(chunk, offset);
    }
}

u64 disassemble(Chunk& chunk, size_t offset)
{
    std::println("{:04d}", offset);

    u8 raw = chunk.get(offset);
    OpCode instruction = static_cast<OpCode>(raw);
    switch (instruction) {
        case OpCode::RETURN:
            return internal::simple_instr("RETURN", offset);
        default:
            std::println("Unknown opcode {:d}", raw);
            return offset + 1;
    }
}

}