#ifndef src_debug_h
#define src_debug_h

#include "chunk.h"
#include "common.h"
#include <string_view>

namespace debug
{

// disassembles chunk
void disassemble(Chunk& chunk, std::string_view name);
// disassembles instruction
[[nodiscard]] u64 disassemble(Chunk& chunk, size_t offset);

}

#endif // src_debug_h