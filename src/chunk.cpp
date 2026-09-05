#include "chunk.h"
#include "line.h"
#include "memory.h"
#include "opcode.h"
#include "value.h"
#include <cstdint>
#include <unistd.h>
#include <utility>

Chunk::Chunk() : code(), constants() { code.reserve(BASE_CHUNK_CAPACITY); }

void Chunk::write_constant(value val, u64 line) {
    size_t idx = add_constant(val);
    if (idx <= UINT8_MAX) {
        write(std::to_underlying(OpCode::CONSTANT_LONG), line);
        write(static_cast<u8>(idx), line);
    } else {
        write(std::to_underlying(OpCode::CONSTANT_LONG), line);
        write(static_cast<u8>(idx & 0xFF), line);
        write(static_cast<u8>(idx >> 8 & 0xFF), line);
        write(static_cast<u8>(idx >> 16 & 0xFF), line);
    }
}

void Chunk::write(const u8& byte, u64 line) {
    if (lines.empty() || lines.back().get_line() != line) {
        lines.push_back(Line(line, code.size()));
    }
    code.push_back(byte);
}

size_t Chunk::add_constant(value val) {
    constants.write(std::move(val));
    // This may overflow
    return constants.count() - 1;
}

i64 Chunk::get_line(size_t offset) {
    for (const auto& line : lines) {
        if (offset == line.get_offset()) {
            return static_cast<i64>(line.get_line());
        }
    }
    return -1;
}
