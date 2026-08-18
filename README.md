# loxpp

A minimal C++23 implementation of a Lox bytecode compiler inspired by Robert Nystrom's Crafting Interpreters.

This project focuses on the compiler side of Lox: tokenization, parsing, bytecode generation, and a simple runtime model, implemented in a compact C++23 codebase.

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/loxpp
```

The program reads and executes Lox source from standard input or the provided entry points.
