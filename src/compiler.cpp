#include "compiler.h"

void Compiler::compile(std::string_view source) { scanner->init(source); }
