#ifndef debug_h
#define debug_h

#include "chunk.h"
#include "common.h"
#include <string_view>

#define DEBUG 1
#define DEBUG_PRINT_CODE 1

namespace debug {

// disassembles chunk
void disassemble(Chunk& chunk, std::string_view name);
// disassembles instruction
u64 disassemble(Chunk& chunk, size_t offset);

} // namespace debug

#endif // debug_h
