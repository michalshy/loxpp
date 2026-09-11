#ifndef memory_h
#define memory_h

#include "common.h"
#include "object.h"

constexpr u8 BASE_CHUNK_CAPACITY = 8;

void free_objects(Object* head);

#endif // memory_h
