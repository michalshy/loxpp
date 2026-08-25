#include "common.h"
#include "chunk.h"
#include "debug.h"

int main()
{
    Chunk chunk{};

    chunk.write(OpCode::RETURN);
    debug::disassemble(chunk, "test_chunk");
    return 0;
}