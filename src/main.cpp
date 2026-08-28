#include "chunk.h"
#include "common.h"
#include "debug.h"

int main() {
    Chunk chunk{};

    u8 cnst = chunk.add_constant(1.2);

    chunk.write(OpCode::CONSTANT, 123);
    chunk.write(cnst, 123);

    chunk.write(OpCode::RETURN, 124);

    debug::disassemble(chunk, "test_chunk");

    return 0;
}
