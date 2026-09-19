#include "vm.h"
#include "chunk.h"
#include "memory.h"
#include "object.h"
#include "opcode.h"
#include "value.h"
#include <expected>
#include <print>
#include <string_view>
#include <type_traits>
#include <variant>

VM::~VM() { free_objects(objects); }

std::expected<void, InterpretError> VM::interpret(std::string_view source) {
    Chunk chunk{};

    if (!compiler->compile(source, chunk)) {
        return std::unexpected(InterpretError::COMPILE_ERROR);
    }

    curr_chunk = &chunk;
    ip = 0;

    return run();
}

std::expected<void, InterpretError> VM::interpret(Chunk& chunk) {
    curr_chunk = &chunk;
    ip = 0;
    std::println("interpreting...");
    return run();
}

std::expected<void, InterpretError> VM::run() {
    bool exit{false};
    if (!curr_chunk) {
        return std::unexpected(InterpretError::RUNTIME_ERROR);
    }

    while (!exit) {
        OpCode instruction = static_cast<OpCode>(curr_chunk->byte(ip++));
        switch (instruction) {
        case OpCode::RETURN:
            return {};
        case OpCode::CONSTANT: {
            size_t idx = detail::constant_idx(*curr_chunk, ip);
            stack.push(curr_chunk->constant(idx));
            ip++;
            break;
        }
        case OpCode::FALSE:
            stack.push(false);
            break;
        case OpCode::TRUE:
            stack.push(true);
            break;
        case OpCode::NIL:
            stack.push(std::monostate());
            break;
        case OpCode::ADD:
            add();
            break;
        case OpCode::SUBTRACT:
            subtract();
            break;
        case OpCode::MULTIPLY:
            multiply();
            break;
        case OpCode::DIVIDE:
            divide();
            break;
        case OpCode::NOT:
            std::visit(overloaded{[&](std::monostate) { return; },
                                  [&](bool b) { stack.push(!b); },
                                  [&](double) { return; },
                                  [&](Object*) { return; }},
                       pop_stack());
            break;
        case OpCode::NEGATE: {
            if (!std::holds_alternative<double>(stack.top())) {
                return std::unexpected(InterpretError::RUNTIME_ERROR);
            }
            stack.top() = std::get<double>(stack.top()) * -1;
            break;
        }
        case OpCode::EQUAL: {
            value a = pop_stack();
            value b = pop_stack();
            stack.push(a == b);
            break;
        }
        case OpCode::LESS: {
            value a = pop_stack();
            value b = pop_stack();
            stack.push(std::is_same_v<std::decay_t<decltype(a)>,
                                      std::decay_t<decltype(a)>> &&
                       a > b);
            break;
        }
        case OpCode::GREATER: {
            value a = pop_stack();
            value b = pop_stack();
            stack.push(std::is_same_v<std::decay_t<decltype(a)>,
                                      std::decay_t<decltype(a)>> &&
                       a < b);
            break;
        }
        case OpCode::PRINT: {
            PrintVal(pop_stack());
            std::println();
            break;
        }
        case OpCode::POP:
            pop_stack();
            break;
        case OpCode::DEFINE_GLOBAL: {
            size_t idx = detail::constant_idx(*curr_chunk, ip);
            value val = curr_chunk->constant(idx);
            StringObject* name =
                static_cast<StringObject*>(std::get<Object*>(val));

            globals[name->value()] = stack.top();
            pop_stack();
            ip++;
            break;
        }
        case OpCode::SET_GLOBAL: {
            size_t idx = detail::constant_idx(*curr_chunk, ip);
            ip++;
            value val = curr_chunk->constant(idx);
            StringObject* name =
                static_cast<StringObject*>(std::get<Object*>(val));

            auto it = globals.find(name->value());
            if (it == globals.end()) {
                runtime_error("Undefined variable '{}'.", name->value());
                return std::unexpected(InterpretError::RUNTIME_ERROR);
            }
            it->second = stack.top();
            break;
        }
        case OpCode::GET_GLOBAL: {
            size_t idx = detail::constant_idx(*curr_chunk, ip);
            ip++;
            value val = curr_chunk->constant(idx);
            StringObject* name =
                static_cast<StringObject*>(std::get<Object*>(val));

            auto it = globals.find(name->value());
            if (it == globals.end()) {
                runtime_error("Undefined variable '{}'.", name->value());
                return std::unexpected(InterpretError::RUNTIME_ERROR);
            }
            stack.push(it->second);
            break;
        }
        default:
            break;
        }
    }

    return std::unexpected(InterpretError::RUNTIME_ERROR);
}

value VM::pop_stack() {
    value v = stack.top();
    stack.pop();
    return v;
}

void VM::add() {
    value a = pop_stack();
    value b = pop_stack();

    if (std::holds_alternative<double>(a) &&
        std::holds_alternative<double>(b)) {
        stack.push(std::get<double>(b) + std::get<double>(a));
        return;
    }

    if (std::holds_alternative<Object*>(a) &&
        std::holds_alternative<Object*>(b) &&
        std::get<Object*>(a)->IsType(ObjectType::STRING) &&
        std::get<Object*>(b)->IsType(ObjectType::STRING)) {
        StringObject* a_str = static_cast<StringObject*>(std::get<Object*>(a));
        StringObject* b_str = static_cast<StringObject*>(std::get<Object*>(b));
        stack.push(
            allocate_object<StringObject>(b_str->value(), a_str->value()));
        return;
    }

    runtime_error("Operands must be string or number.");
}

void VM::subtract() {
    value a = pop_stack();
    value b = pop_stack();

    if (std::holds_alternative<double>(a) &&
        std::holds_alternative<double>(b)) {
        stack.push(std::get<double>(b) - std::get<double>(a));
    }
}

void VM::divide() {
    value a = pop_stack();
    value b = pop_stack();

    if (std::holds_alternative<double>(a) &&
        std::holds_alternative<double>(b)) {
        stack.push(std::get<double>(b) / std::get<double>(a));
    }
}

void VM::multiply() {
    value a = pop_stack();
    value b = pop_stack();

    if (std::holds_alternative<double>(a) &&
        std::holds_alternative<double>(b)) {
        stack.push(std::get<double>(b) * std::get<double>(a));
    }
}

namespace detail {

size_t constant_idx(Chunk& chunk, size_t offset) {
    return static_cast<size_t>(chunk.byte(offset));
}
} // namespace detail
