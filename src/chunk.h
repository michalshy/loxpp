#ifndef src_chunk_h
#define src_chunk_h

#include "common.h"
#include "value.h"
#include "line.h"
#include <deque>

enum class OpCode : u8 {
    CONSTANT,
    RETURN,
};

class Chunk
{
    std::vector<u8> code{};
    std::deque<Line> lines{};
    Values constants{};
public:
    Chunk();

    void write(const u8& byte, u32 line);
    void write(const OpCode& op, u32 line);

    [[nodiscard]] u8 add_constant(value val);

    [[nodiscard]] size_t size() { return code.size(); }
    [[nodiscard]] const u8& byte(size_t offset) { return code[offset]; }
    [[nodiscard]] const value& constant(u8 idx) { return constants.get(idx); }
    [[nodiscard]] u64 get_line(size_t offset);
private:
    void insert_line(u64 line);
};

#endif // src_chunk_h