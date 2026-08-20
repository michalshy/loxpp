#ifndef src_chunk_h
#define src_chunk_h

#include "common.h"

enum class OpCode : u8 {
    RETURN,
};

class Chunk
{
    std::vector<u8> code;
public:
    Chunk();

    void write(const u8& byte);
    void write(const OpCode& op);
};

#endif // src_chunk_h