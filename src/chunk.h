#ifndef chunk_h
#define chunk_h

#include "common.h"
#include "line.h"
#include "value.h"
#include <vector>

enum class OpCode : u8 {
    CONSTANT,
    RETURN,
};

class Chunk {
    std::vector<u8> code{};
    std::vector<Line> lines{};
    Values constants{};

  public:
    Chunk();

    void write(const u8& byte, u32 line);
    void write(const OpCode& op, u32 line);

    [[nodiscard]] u8 add_constant(value val);

    [[nodiscard]] size_t size() { return code.size(); }
    [[nodiscard]] const u8& byte(size_t offset) { return code[offset]; }
    [[nodiscard]] const value& constant(u8 idx) { return constants.get(idx); }
    [[nodiscard]] i64 get_line(size_t offset);

  private:
    void insert_line(u64 line);
};

#endif // chunk_h
