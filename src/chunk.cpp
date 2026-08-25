#include "chunk.h"
#include "memory.h"
#include <utility>

Chunk::Chunk()
    : code(BASE_CHUNK_CAPACITY)
    , constants()
{
}

void Chunk::write(const u8& byte)
{
    code.push_back(byte);
}

void Chunk::write(const OpCode& op)
{
    code.push_back(std::to_underlying(op));
}