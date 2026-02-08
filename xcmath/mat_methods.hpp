#pragma once
#include <type_traits>

#include "./functions.hpp"
#include "methods.hpp"
#include "number_meta.hpp"

#define self (*static_cast<Derived*>(this))
#define const_self (*static_cast<const Derived*>(this))
#define require_method(method)                     \
    static_assert(is_impl_method<Derived, method>, \
                  "Derived must be derived from " #method)

namespace xcmath {

METHOD_DEF_BEGIN(trace_method)
constexpr auto trace() const noexcept {
    using result_type = std::decay_t<decltype(const_self[0, 0])>;
    result_type result = number_meta::number_properties<result_type>::zero;
    auto size = const_self.size() < const_self[0].size() ? const_self.size()
                                                         : const_self[0].size();
    for (size_t i = 0; i < size; ++i) {
        result += const_self[i, i];
    }
    return result;
}
METHOD_DEF_END()

// determinant_method - base template (generic Gaussian elimination for any
// size)
METHOD_DEF_BEGIN(determinant_method)
constexpr auto determinant() const noexcept {
    using result_type = std::decay_t<decltype(const_self[0, 0])>;
    constexpr size_t size_ = sizeof(Derived) == 0 ? 0 : 0;  // placeholder
    static_assert(sizeof(Derived) == 0,
                  "determinant() requires matrix specialization");
    return result_type{};
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(transpose_method)
constexpr auto transpose() const noexcept {
    using value_type = std::decay_t<decltype(const_self[0, 0])>;
    constexpr size_t row = traits::mat_dims<Derived>::rows;
    constexpr size_t col = traits::mat_dims<Derived>::cols;
    constexpr bool is_col_major = traits::mat_dims<Derived>::is_col_major;
    mat<value_type, col, row, !is_col_major> result;
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            result[j, i] = const_self[i, j];
        }
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(inverse_method)
constexpr auto inverse() const noexcept {
    using value_type = std::decay_t<decltype(const_self[0, 0])>;
    constexpr size_t row = traits::mat_dims<Derived>::rows;
    constexpr size_t col = traits::mat_dims<Derived>::cols;
    constexpr bool is_col_major = traits::mat_dims<Derived>::is_col_major;
    static_assert(row == col, "inverse() only valid for square matrices");

    mat<value_type, row, row, is_col_major> result;

    // 2x2 matrix inverse
    if constexpr (row == 2 && col == 2) {
        value_type det = const_self[0, 0] * const_self[1, 1] -
                         const_self[0, 1] * const_self[1, 0];
        result[0, 0] = const_self[1, 1] / det;
        result[0, 1] = -const_self[0, 1] / det;
        result[1, 0] = -const_self[1, 0] / det;
        result[1, 1] = const_self[0, 0] / det;
    }
    // 3x3 matrix inverse
    else if constexpr (row == 3 && col == 3) {
        value_type det = xcmath::determinant(
            mat<value_type, 3, 3, is_col_major>{const_self});

        // Compute cofactors and adjugate matrix
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                // Build 2x2 minor
                mat<value_type, 2, 2, is_col_major> minor;
                size_t minor_row = 0;
                for (size_t r = 0; r < 3; ++r) {
                    if (r == i) continue;
                    size_t minor_col = 0;
                    for (size_t c = 0; c < 3; ++c) {
                        if (c == j) continue;
                        minor[minor_row, minor_col] = const_self[r, c];
                        ++minor_col;
                    }
                    ++minor_row;
                }
                value_type cofactor =
                    xcmath::determinant(minor) * ((i + j) % 2 == 0 ? 1 : -1);
                result[j, i] =
                    cofactor / det;  // Adjugate = transpose of cofactor
            }
        }
    }
    // 4x4 matrix inverse
    else if constexpr (row == 4 && col == 4) {
        value_type det = xcmath::determinant(
            mat<value_type, 4, 4, is_col_major>{const_self});

        // Compute cofactors and adjugate matrix
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                // Build 3x3 minor
                mat<value_type, 3, 3, is_col_major> minor;
                size_t minor_row = 0;
                for (size_t r = 0; r < 4; ++r) {
                    if (r == i) continue;
                    size_t minor_col = 0;
                    for (size_t c = 0; c < 4; ++c) {
                        if (c == j) continue;
                        minor[minor_row, minor_col] = const_self[r, c];
                        ++minor_col;
                    }
                    ++minor_row;
                }
                value_type cofactor =
                    xcmath::determinant(minor) * ((i + j) % 2 == 0 ? 1 : -1);
                result[j, i] =
                    cofactor / det;  // Adjugate = transpose of cofactor
            }
        }
    }
    // Generic size inverse using Gaussian elimination
    else {
        constexpr size_t size = row;
        result = xcmath::inverse(const_self);
    }

    return result;
}
METHOD_DEF_END()

}  // namespace xcmath

#undef self
#undef const_self
#undef require_method
