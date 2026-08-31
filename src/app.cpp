#include "app.h"
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <print>
#include <string>

#include "chunk.h"
#include "common.h"
#include "fs.h"
#include "opcode.h"
#include "vm.h"

void App::run(int argc, const char* argv[]) {
    Mode mode = check_mode(argc);
    if (mode == Mode::REPL) {
        repl();
    } else if (mode == Mode::EXEC) {
        run_file(argv[1]);
    } else {
        std::println("Usage: loxpp [path]");
    }
}

void App::repl() {
    bool exit{false};
    std::string line;

    while (!exit) {
        std::print(">  ");
        if (std::getline(std::cin, line)) {
            VM::Instance().interpret(line);
        } else {
            std::println();
        }
    }
}

void App::run_file(std::filesystem::path path) {
    auto content = flox::read_file(path);
    if (!content) {
        std::println("Error during file reading: {}",
                     static_cast<u8>(content.error()));
    }

    std::expected<void, InterpretError> result =
        VM::Instance().interpret(content.value());

    // check result
}

Mode App::check_mode(int no) {
    switch (no) {
    case 1:
        return Mode::REPL;
    case 2:
        return Mode::EXEC;
    default:
        return Mode::UNDEFINED;
    }
}
