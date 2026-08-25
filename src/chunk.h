#ifndef src_chunk_h
#define src_chunk_h

#include "common.h"
#include "value.h"

enum class OpCode : u8 {
    RETURN,
};

class Chunk
{
    std::vector<u8> code{};
    Values constants{};
public:
    Chunk();

    void write(const u8& byte);
    void write(const OpCode& op);

    size_t size() { return code.size(); }
    u8 get(size_t offset) { return code[offset]; }
};

#endif // src_chunk_h