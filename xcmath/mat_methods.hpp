#pragma once
#include <type_traits>

#include "./functions.hpp"
#include "./methods.hpp"
#include "./number_meta.hpp"
#include "./traits.hpp"

#define self (*static_cast<Derived*>(this))
#define const_self (*static_cast<const Derived*>(this))
#define require_method(method)                     \
    static_assert(is_impl_method<Derived, method>, \
                  "Derived must be derived from " #method)

namespace xcmath {
METHOD_DEF_BEGIN(trace_method)
constexpr auto trace() const noexcept {
    using result_type = std::decay_t<decltype(const_self.at(0, 0))>;
    result_type result = number_meta::number_properties<result_type>::zero;
    auto size = const_self.size() < const_self[0].size() ? const_self.size()
                                                         : const_self[0].size();
    for (size_t i = 0; i < size; ++i) {
        result += const_self.at(i, i);
    }
    return result;
}
METHOD_DEF_END()

// determinant_method - base template (generic Gaussian elimination for any
// size)
METHOD_DEF_BEGIN(determinant_method)
constexpr auto determinant() const noexcept {
    using result_type = std::decay_t<decltype(const_self.at(0, 0))>;
    constexpr size_t size_ = sizeof(Derived) == 0 ? 0 : 0;  // placeholder
    static_assert(sizeof(Derived) == 0,
                  "determinant() requires matrix specialization");
    return result_type{};
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(transpose_method)
constexpr auto transpose() const noexcept {
    using value_type = std::decay_t<decltype(const_self.at(0, 0))>;
    constexpr size_t row = traits::mat_dims<Derived>::rows;
    constexpr size_t col = traits::mat_dims<Derived>::cols;
    constexpr bool is_col_major = traits::mat_dims<Derived>::is_col_major;
    mat<value_type, col, row, !is_col_major> result;
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            result.at(j, i) = const_self.at(i, j);
        }
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(inverse_method)
constexpr auto inverse() const noexcept {
    using value_type = std::decay_t<decltype(const_self.at(0, 0))>;
    constexpr size_t row = traits::mat_dims<Derived>::rows;
    constexpr size_t col = traits::mat_dims<Derived>::cols;
    constexpr bool is_col_major = traits::mat_dims<Derived>::is_col_major;
    static_assert(row == col, "inverse() only valid for square matrices");

    mat<value_type, row, row, is_col_major> result;

    // 2x2 matrix inverse
    if constexpr (row == 2 && col == 2) {
        value_type det = const_self.at(0, 0) * const_self.at(1, 1) -
                         const_self.at(0, 1) * const_self.at(1, 0);
        result.at(0, 0) = const_self.at(1, 1) / det;
        result.at(0, 1) = -const_self.at(0, 1) / det;
        result.at(1, 0) = -const_self.at(1, 0) / det;
        result.at(1, 1) = const_self.at(0, 0) / det;
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
                        minor.at(minor_row, minor_col) = const_self.at(r, c);
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
                        minor.at(minor_row, minor_col) = const_self.at(r, c);
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
        // Derived a = const_self.clone();
        // Derived aug{};
        // static constexpr size_t size_ = traits::mat_dims<Derived>::rows;
        // using T = decltype(const_self[0, 0]);
        // // Build augmented matrix [A|I]
        // for (size_t i = 0; i < size_; ++i) {
        //     for (size_t j = 0; j < size_; ++j) {
        //         aug[i, j] = a[i, j];
        //         aug[i, j + size_] = (i == j) ? T{1} : T{};
        //     }
        // }

        // // Gaussian elimination with partial pivoting
        // for (size_t i = 0; i < size_; ++i) {
        //     // Find pivot
        //     size_t pivot = i;
        //     for (size_t r = i + 1; r < size_; ++r) {
        //         if (std::fabs(aug[r, i]) > std::fabs(aug[pivot, i])) {
        //             pivot = r;
        //         }
        //     }

        //     // Swap rows if needed
        //     if (pivot != i) {
        //         for (size_t c = i; c < 2 * size_; ++c) {
        //             std::swap(aug[i, c], aug[pivot, c]);
        //         }
        //     }

        //     // Check for singular matrix
        //     if (std::fabs(aug[i, i]) == T{}) {
        //         return const_self.clone();  // Singular matrix, return
        //         original
        //     }

        //     // Scale pivot row
        //     T pivot_val = aug[i, i];
        //     for (size_t c = i; c < 2 * size_; ++c) {
        //         aug[i, c] /= pivot_val;
        //     }

        //     // Eliminate column
        //     for (size_t r = 0; r < size_; ++r) {
        //         if (r == i) continue;
        //         T factor = aug[r, i];
        //         for (size_t c = i; c < 2 * size_; ++c) {
        //             aug[r, c] -= factor * aug[i, c];
        //         }
        //     }
        // }

        // // Extract inverse from augmented matrix
        // Derived result{};
        // for (size_t i = 0; i < size_; ++i) {
        //     for (size_t j = 0; j < size_; ++j) {
        //         result[i, j] = aug[i, j + size_];
        //     }
        // }

        // return result;
    }

    return result;
}
METHOD_DEF_END()

METHOD_DECLARE(rotate_method);
METHOD_DECLARE(translate_method);
METHOD_DECLARE(scale_method);
FACTORY_DEF_BEGIN(look_at_method)
template <typename T>
    requires(traits::length_properties<T>::length == 3)
static constexpr auto look_at(const T& eye, const T& center, const T& up) {
    auto result = Derived::unit();
    const auto forward = (center - eye).normalize();
    const auto right = up.cross(forward).normalize();
    const auto new_up = forward.cross(right);
    result.at(0, 0) = right[0];
    result.at(0, 1) = right[1];
    result.at(0, 2) = right[2];
    result.at(1, 0) = new_up[0];
    result.at(1, 1) = new_up[1];
    result.at(1, 2) = new_up[2];
    result.at(2, 0) = -forward[0];
    result.at(2, 1) = -forward[1];
    result.at(2, 2) = -forward[2];
    return result;
}
FACTORY_DEF_END();

using mat_transform_methods_recorder =
    method_recorder<rotate_method, translate_method, scale_method>;

template <typename T, bool is_col_major_>
    requires(std::is_floating_point_v<T>)
struct special_mat_ext_methods_recorder<T, 2, 2, is_col_major_>
    : impl_special_mat_ext_methods<rotate_method> {};
template <typename T, bool is_col_major_>
    requires(std::is_floating_point_v<T>)
struct special_mat_ext_methods_recorder<T, 3, 3, is_col_major_>
    : details::return_type<mat_transform_methods_recorder> {};
template <typename T, bool is_col_major_>
    requires(std::is_floating_point_v<T>)
struct special_mat_ext_methods_recorder<T, 4, 4, is_col_major_>
    : details::return_type<
          mat_transform_methods_recorder::push_back<look_at_method>> {};
}  // namespace xcmath

#undef self
#undef const_self
#undef require_method
