#pragma once
#include <initializer_list>
#include <type_traits>

#include "./alias.hpp"
#include "./mat_methods.hpp"
#include "./vec.hpp"
#include "methods.hpp"

namespace xcmath {
using comman_mat_ext_methods =
    method_recorder<trace_method, determinant_method, transpose_method,
                    inverse_method>;
namespace details {
template <typename T, size_t row_, size_t col_, bool is_col_major_,
          typename ext_method_recorder = comman_mat_ext_methods>
struct base_of_mat_impl_helper;
template <typename T, size_t row_, size_t col_, bool is_col_major_,
          template <typename, typename> typename... ext_methods>
struct base_of_mat_impl_helper<T, row_, col_, is_col_major_,
                               method_recorder<ext_methods...>> {
    using type =
        std::conditional_t<is_col_major_,
                           vec_impl<mat<T, row_, col_, is_col_major_>,
                                    vec<T, row_>, col_, ext_methods...>,
                           vec_impl<mat<T, row_, col_, is_col_major_>,
                                    vec<T, col_>, row_, ext_methods...>>;
};

template <typename T, size_t row_, size_t col_, bool is_col_major_>
using base_of_mat_impl =
    base_of_mat_impl_helper<T, row_, col_, is_col_major_>::type;
};  // namespace details

template <typename T, size_t row_, size_t col_, bool is_col_major_>
struct mat_impl
    : public details::base_of_mat_impl<T, row_, col_, is_col_major_> {
   private:
    using mat = mat<T, row_, col_, is_col_major_>;

   protected:
    using Super = details::base_of_mat_impl<T, row_, col_, is_col_major_>;

   public:
    using Super::Super;
};

template <typename T, size_t row_, size_t col_>
class mat<T, row_, col_, false> : public mat_impl<T, row_, col_, false> {
    using impl = mat_impl<T, row_, col_, false>;
    using typename impl::Super;

   public:
    using impl::impl;

    constexpr mat& operator=(const mat<T, row_, col_, true>& other);
    constexpr T& operator[](size_t i, size_t j) { return Super::data_[i][j]; }
    constexpr const T& operator[](size_t i, size_t j) const {
        return Super::data_[i][j];
    }
    constexpr vec<T, col_>& operator[](size_t i) { return Super::data_[i]; }
    constexpr const vec<T, col_>& operator[](size_t i) const {
        return Super::data_[i];
    }
};

template <typename T, size_t row_, size_t col_>
class mat<T, row_, col_, true> : public mat_impl<T, row_, col_, true> {
    using impl = mat_impl<T, row_, col_, true>;
    using typename impl::Super;
    using row_view = vec_view<T, col_, row_>;
    using const_row_view = const_vec_view<T, col_, row_>;

   public:
    using impl::impl;

    constexpr mat(const std::initializer_list<vec<T, col_>>& init) : impl{} {
        size_t i = 0;
        for (const auto& row : init) {
            for (size_t j = 0; j < col_; ++j) {
                Super::data_[j][i] = row[j];
            }
            ++i;
        }
    }

    constexpr row_view operator[](size_t idx) {
        assert_index(idx, row_);
        return row_view(&operator[](idx, 0));
    }
    constexpr const_row_view operator[](size_t idx) const {
        assert_index(idx, row_);
        return const_row_view(&operator[](idx, 0));
    }
    constexpr T& operator[](size_t i, size_t j) { return Super::data_[j][i]; }
    constexpr const T& operator[](size_t i, size_t j) const {
        return Super::data_[j][i];
    }
    constexpr mat& operator=(const mat<T, row_, col_, false>& other) {
        for (size_t i = 0; i < row_; ++i) {
            for (size_t j = 0; j < col_; ++j) {
                this->data_[j][i] = other[i, j];
            }
        }
        return *this;
    }
};
template <typename Base, typename T, size_t row_, size_t col_,
          bool is_col_major_>
struct size_method<Base, mat<T, row_, col_, is_col_major_>> : Base {
    inline constexpr size_t size() const noexcept {
        return is_col_major_ ? col_ : row_;
    }
};

// determinant_method specialization for 2x2 matrix
template <typename Base, typename T, bool is_col_major_>
struct determinant_method<Base, mat<T, 2, 2, is_col_major_>> : Base {
    inline constexpr T determinant() const noexcept {
        const auto& self =
            *static_cast<const mat<T, 2, 2, is_col_major_>*>(this);
        return self[0, 0] * self[1, 1] - self[0, 1] * self[1, 0];
    }
};

// determinant_method specialization for 3x3 matrix
template <typename Base, typename T, bool is_col_major_>
struct determinant_method<Base, mat<T, 3, 3, is_col_major_>> : Base {
    inline constexpr T determinant() const noexcept {
        const auto& self =
            *static_cast<const mat<T, 3, 3, is_col_major_>*>(this);
        return self[0, 0] *
                   (self[1, 1] * self[2, 2] - self[1, 2] * self[2, 1]) -
               self[0, 1] *
                   (self[1, 0] * self[2, 2] - self[1, 2] * self[2, 0]) +
               self[0, 2] * (self[1, 0] * self[2, 1] - self[1, 1] * self[2, 0]);
    }
};

// determinant_method specialization for 4x4 matrix
template <typename Base, typename T, bool is_col_major_>
struct determinant_method<Base, mat<T, 4, 4, is_col_major_>> : Base {
    inline constexpr T determinant() const noexcept {
        const auto& self =
            *static_cast<const mat<T, 4, 4, is_col_major_>*>(this);
        return self[0, 0] *
                   (self[1, 1] *
                        (self[2, 2] * self[3, 3] - self[2, 3] * self[3, 2]) -
                    self[1, 2] *
                        (self[2, 1] * self[3, 3] - self[2, 3] * self[3, 1]) +
                    self[1, 3] *
                        (self[2, 1] * self[3, 2] - self[2, 2] * self[3, 1])) -
               self[0, 1] *
                   (self[1, 0] *
                        (self[2, 2] * self[3, 3] - self[2, 3] * self[3, 2]) -
                    self[1, 2] *
                        (self[2, 0] * self[3, 3] - self[2, 3] * self[3, 0]) +
                    self[1, 3] *
                        (self[2, 0] * self[3, 2] - self[2, 2] * self[3, 0])) +
               self[0, 2] *
                   (self[1, 0] *
                        (self[2, 1] * self[3, 3] - self[2, 3] * self[3, 1]) -
                    self[1, 1] *
                        (self[2, 0] * self[3, 3] - self[2, 3] * self[3, 0]) +
                    self[1, 3] *
                        (self[2, 0] * self[3, 1] - self[2, 1] * self[3, 0])) -
               self[0, 3] *
                   (self[1, 0] *
                        (self[2, 1] * self[3, 2] - self[2, 2] * self[3, 1]) -
                    self[1, 1] *
                        (self[2, 0] * self[3, 2] - self[2, 2] * self[3, 0]) +
                    self[1, 2] *
                        (self[2, 0] * self[3, 1] - self[2, 1] * self[3, 0]));
    }
};

// determinant_method specialization for generic NxN matrix (Gaussian
// elimination)
template <typename Base, typename T, size_t size_, bool is_col_major_>
struct determinant_method<Base, mat<T, size_, size_, is_col_major_>> : Base {
    inline constexpr T determinant() const noexcept {
        const auto& self =
            *static_cast<const mat<T, size_, size_, is_col_major_>*>(this);
        // Copy matrix to avoid modifying original
        mat<T, size_, size_, is_col_major_> a = self;

        T det = number_meta::number_properties<T>::unit;
        for (size_t i = 0; i < size_; ++i) {
            // Find pivot
            size_t pivot = i;
            for (size_t r = i + 1; r < size_; ++r) {
                if (xcmath::fabs(a[r, i]) > xcmath::fabs(a[pivot, i])) {
                    pivot = r;
                }
            }

            // If pivot is zero, matrix is singular
            if (a[pivot, i] == number_meta::number_properties<T>::zero) {
                return number_meta::number_properties<T>::zero;
            }

            // Swap rows if needed
            if (pivot != i) {
                for (size_t c = i; c < size_; ++c) {
                    std::swap(a[i, c], a[pivot, c]);
                }
                det = -det;  // Row swap changes sign
            }

            // Elimination
            for (size_t r = i + 1; r < size_; ++r) {
                if (a[r, i] == number_meta::number_properties<T>::zero)
                    continue;
                T factor = a[r, i] / a[i, i];
                for (size_t c = i; c < size_; ++c) {
                    a[r, c] -= factor * a[i, c];
                }
            }
        }

        // Calculate product of diagonal
        for (size_t i = 0; i < size_; ++i) {
            det *= a[i, i];
        }

        return det;
    }
};

namespace number_meta {
template <typename T, size_t row_, size_t col_, bool is_col_major_>
struct number_properties<mat<T, row_, col_, is_col_major_>> {
    using number_type = mat<T, row_, col_, is_col_major_>;
    static constexpr number_type zero{};
    static constexpr number_type unit = []() {
        number_type unit = zero;
        for (size_t i = 0; i < (col_ < row_ ? col_ : row_); ++i) {
            unit[i, i] = number_properties<T>::unit;
        }
        return unit;
    }();
};
}  // namespace number_meta

template <typename T, size_t row_, size_t col_>
inline constexpr mat<T, row_, col_, false>&
mat<T, row_, col_, false>::operator=(const mat<T, row_, col_, true>& other) {
    for (size_t i = 0; i < row_; ++i) {
        for (size_t j = 0; j < col_; ++j) {
            this->data_[i][j] = other[i, j];
        }
    }
    return *this;
}

}  // namespace xcmath

// Specialize mat_dims for all matrix types
template <typename T, size_t row_, size_t col_, bool is_col_major_>
struct xcmath::mat_dims<xcmath::mat<T, row_, col_, is_col_major_>> {
    static constexpr size_t rows = row_;
    static constexpr size_t cols = col_;
    static constexpr bool is_col_major = is_col_major_;
};