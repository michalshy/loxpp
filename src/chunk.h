#ifndef chunk_h
#define chunk_h

#include "common.h"
#include "line.h"
#include "opcode.h"
#include "value.h"
#include <vector>

class Chunk {
    std::vector<u8> code{};
    std::vector<Line> lines{};
    Values constants{};

  public:
    Chunk();

    void write_constant(value val, u64 line);

    void write(const u8& byte, u64 line);
    void write(const OpCode& op, u64 line);

    [[nodiscard]] size_t add_constant(value val);

    [[nodiscard]] size_t size() { return code.size(); }
    [[nodiscard]] const u8& byte(size_t offset) { return code[offset]; }
    [[nodiscard]] const value& constant(size_t idx) {
        return constants.get(idx);
    }
    [[nodiscard]] i64 get_line(size_t offset);
};

#endif // chunk_h
