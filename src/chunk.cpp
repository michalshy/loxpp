#include "chunk.h"
#include "line.h"
#include "memory.h"
#include "value.h"
#include <utility>

Chunk::Chunk() : code(), constants() { code.reserve(BASE_CHUNK_CAPACITY); }

void Chunk::write(const u8& byte, u32 line) {
    if (lines.empty() || lines.back().get_line() != line) {
        lines.push_back(Line(line, code.size()));
    }
    code.push_back(byte);
}

void Chunk::write(const OpCode& op, u32 line) {
    if (lines.empty() || lines.back().get_line() != line) {
        lines.push_back(Line(line, code.size()));
    }
    code.push_back(std::to_underlying(op));
}

u8 Chunk::add_constant(value val) {
    constants.write(std::move(val));
    // This may overflow
    return static_cast<u8>(constants.count() - 1);
}

i64 Chunk::get_line(size_t offset) {
    for (const auto& line : lines) {
        if (offset == line.get_offset()) {
            return static_cast<i64>(line.get_line());
        }
    }
    return -1;
}
