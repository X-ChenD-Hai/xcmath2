#pragma once
#include <type_traits>
#include <xcmixin/xcmixin.hpp>

#include "./functions.hpp"
#include "./number_meta.hpp"
#include "./traits.hpp"
#include "xcmath/functions.hpp"


namespace xcmath {
XCMIXIN_DEF_BEGIN(trace_method)
constexpr auto trace() const noexcept {
    using result_type = std::decay_t<decltype(xcmixin_const_self.at(0, 0))>;
    result_type result = number_meta::number_properties<result_type>::zero;
    auto size = xcmixin_const_self.size() < xcmixin_const_self[0].size() ? xcmixin_const_self.size()
                                                         : xcmixin_const_self[0].size();
    for (size_t i = 0; i < size; ++i) {
        result += xcmixin_const_self.at(i, i);
    }
    return result;
}
XCMIXIN_DEF_END()

// determinant_method - base template (generic Gaussian elimination for any
// size)
XCMIXIN_DEF_BEGIN(determinant_method)
constexpr auto determinant() const noexcept {
    using result_type = std::decay_t<decltype(xcmixin_const_self.at(0, 0))>;
    constexpr size_t size_ = sizeof(Derived) == 0 ? 0 : 0;  // placeholder
    static_assert(sizeof(Derived) == 0,
                  "determinant() requires matrix specialization");
    return result_type{};
}
XCMIXIN_DEF_END()

XCMIXIN_DEF_BEGIN(transpose_method)
constexpr auto transpose() const noexcept {
    using value_type = std::decay_t<decltype(xcmixin_const_self.at(0, 0))>;
    constexpr size_t row = traits::mat_dims<Derived>::rows;
    constexpr size_t col = traits::mat_dims<Derived>::cols;
    constexpr bool is_col_major = traits::mat_dims<Derived>::is_col_major;
    mat<value_type, col, row, !is_col_major> result;
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            result.at(j, i) = xcmixin_const_self.at(i, j);
        }
    }
    return result;
}
XCMIXIN_DEF_END()

XCMIXIN_DEF_BEGIN(inverse_method)
constexpr auto inverse() const noexcept {
    using value_type = std::decay_t<decltype(xcmixin_const_self.at(0, 0))>;
    constexpr size_t row = traits::mat_dims<Derived>::rows;
    constexpr size_t col = traits::mat_dims<Derived>::cols;
    constexpr bool is_col_major = traits::mat_dims<Derived>::is_col_major;
    static_assert(row == col, "inverse() only valid for square matrices");

    mat<value_type, row, row, is_col_major> result;

    // 2x2 matrix inverse
    if constexpr (row == 2 && col == 2) {
        value_type det = xcmixin_const_self.at(0, 0) * xcmixin_const_self.at(1, 1) -
                         xcmixin_const_self.at(0, 1) * xcmixin_const_self.at(1, 0);
        result.at(0, 0) = xcmixin_const_self.at(1, 1) / det;
        result.at(0, 1) = -xcmixin_const_self.at(0, 1) / det;
        result.at(1, 0) = -xcmixin_const_self.at(1, 0) / det;
        result.at(1, 1) = xcmixin_const_self.at(0, 0) / det;
    }
    // 3x3 matrix inverse
    else if constexpr (row == 3 && col == 3) {
        value_type det = xcmath::determinant(
            mat<value_type, 3, 3, is_col_major>{xcmixin_const_self});

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
                        minor.at(minor_row, minor_col) = xcmixin_const_self.at(r, c);
                        ++minor_col;
                    }
                    ++minor_row;
                }
                value_type cofactor =
                    xcmath::determinant(minor) * ((i + j) % 2 == 0 ? 1 : -1);
                result.at(j, i) =
                    cofactor / det;  // Adjugate = transpose of cofactor
            }
        }
    }
    // 4x4 matrix inverse
    else if constexpr (row == 4 && col == 4) {
        value_type det = xcmath::determinant(
            mat<value_type, 4, 4, is_col_major>{xcmixin_const_self});

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
                        minor.at(minor_row, minor_col) = xcmixin_const_self.at(r, c);
                        ++minor_col;
                    }
                    ++minor_row;
                }
                value_type cofactor =
                    xcmath::determinant(minor) * ((i + j) % 2 == 0 ? 1 : -1);
                result.at(j, i) =
                    cofactor / det;  // Adjugate = transpose of cofactor
            }
        }
    } else {
        static_assert(false, "inverse() not implemented for this matrix size");
    }

    return result;
}
XCMIXIN_DEF_END()

XCMIXIN_DECLARE(rotate_method);
XCMIXIN_DECLARE(rotate_x_method);
XCMIXIN_DECLARE(rotate_y_method);
XCMIXIN_DECLARE(rotate_z_method);
XCMIXIN_DECLARE(shear_method);
XCMIXIN_DECLARE(translate_method);
XCMIXIN_DECLARE(scale_method);
XCMIXIN_DECLARE(look_at_factory);
XCMIXIN_DECLARE(perspective_factory);
XCMIXIN_DECLARE(ortho_factory);
XCMIXIN_DECLARE(frustum_factory);
using mat4f_ext_methods_recorder =
    xcmixin::mixin_recorder<look_at_factory, perspective_factory,
                             ortho_factory, frustum_factory>;

using mat_transform_methods_recorder =
    xcmixin::mixin_recorder<rotate_method, rotate_x_method, rotate_y_method,
                             rotate_z_method, shear_method, translate_method,
                             scale_method>;

template <typename T, bool is_col_major_>
    requires(std::is_floating_point_v<T>)
struct special_mat_ext_methods_recorder<T, 2, 2, is_col_major_>
    : details::return_type<xcmixin::mixin_recorder<rotate_method>> {};
template <typename T, bool is_col_major_>
    requires(std::is_floating_point_v<T>)
struct special_mat_ext_methods_recorder<T, 3, 3, is_col_major_>
    : details::return_type<mat_transform_methods_recorder> {};
template <typename T, bool is_col_major_>
    requires(std::is_floating_point_v<T>)
struct special_mat_ext_methods_recorder<T, 4, 4, is_col_major_>
    : details::return_type<xcmixin::recorder_concat<
          mat_transform_methods_recorder, mat4f_ext_methods_recorder>> {};
}  // namespace xcmath

