#include "common.h"
#include "chunk.h"

int main()
{
    Chunk chunk{};

    chunk.write(OpCode::RETURN);

    return 0;
}