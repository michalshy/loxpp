#include "chunk.h"
#include "common.h"
#include "opcode.h"
#include "vm.h"

int main() {
    Chunk chunk{};

    u8 cnst = static_cast<u8>(chunk.add_constant(1.2));
    chunk.write(OpCode::CONSTANT, 123);
    chunk.write(cnst, 123);

    cnst = static_cast<u8>(chunk.add_constant(3.4));
    chunk.write(OpCode::CONSTANT, 123);
    chunk.write(cnst, 123);

    chunk.write(OpCode::ADD, 123);

    cnst = static_cast<u8>(chunk.add_constant(5.6));
    chunk.write(OpCode::CONSTANT, 123);
    chunk.write(cnst, 123);

    chunk.write(OpCode::DIVIDE, 123);
    chunk.write(OpCode::NEGATE, 123);

    chunk.write(OpCode::RETURN, 124);

    std::expected<void, InterpretError> res = VM::Instance().interpret(chunk);
    (void)res;

    return 0;
}
