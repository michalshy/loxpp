#include "chunk.h"
#include "memory.h"
#include "value.h"
#include <utility>

Chunk::Chunk()
    : code(BASE_CHUNK_CAPACITY)
    , constants()
{
}

void Chunk::write(const u8& byte, u32 line)
{
    code.push_back(byte);
    insert_line(line);
}

void Chunk::write(const OpCode& op, u32 line)
{
    code.push_back(std::to_underlying(op));
    insert_line(line);
}

u8 Chunk::add_constant(value val)
{
    constants.write(std::move(val));
    // This may overflow
    return static_cast<u8>(constants.count() - 1);
}

// Very suboptimal!
u64 Chunk::get_line(size_t offset)
{
    size_t total_offset = offset;
    size_t idx = 0;
    while(total_offset > 0)
    {
        u64 count = lines[idx].get_count();

        if (total_offset >= count)
        {
            total_offset -= total_offset;
            idx += 1;
        }
        else
        {
            total_offset = 0;
        }
    }
    return lines[idx].get_line();
}

void Chunk::insert_line(u64 line)
{
    if(lines.empty())
    {
        lines.push_back(Line(line));
    }
    else
    {
        auto& el = lines.back();
        if(el.get_line() == line) 
        {
            el.push();
        }
        else 
        {
            lines.push_back(Line(line));
        }
    }
}