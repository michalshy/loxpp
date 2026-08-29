#ifndef concepts_h
#define concepts_h

#include <concepts>

template <typename Op>
concept BinaryOp = requires(Op op, double a, double b) {
    { op(a, b) } -> std::convertible_to<double>;
};

#endif
