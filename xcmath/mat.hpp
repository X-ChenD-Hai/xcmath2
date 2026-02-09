#pragma once
#include <cstddef>
#include <initializer_list>
#include <type_traits>

#include "./alias.hpp"
#include "./mat_methods.hpp"
#include "./vec.hpp"
#include "methods.hpp"
#include "traits.hpp"
#include "xcmath/alias.hpp"

namespace xcmath {
using comman_mat_ext_methods =
    method_recorder<trace_method, determinant_method, transpose_method,
                    inverse_method>;

template <typename T, size_t row_, size_t col_, bool is_col_major_>
struct special_mat_ext_methods_recorder : impl_special_mat_ext_methods<> {};

namespace details {
template <typename T, size_t row_, size_t col_, bool is_col_major_>
using special_mat_ext_methods_recorder =
    dervef_type<special_mat_ext_methods_recorder<T, row_, col_, is_col_major_>>;
template <typename T, size_t row_, size_t col_, bool is_col_major_,
          typename ext_method_recorder = comman_mat_ext_methods,
          typename special_mat_ext_methods_ =
              special_mat_ext_methods_recorder<T, row_, col_, is_col_major_>>
struct base_of_mat_impl_helper;
template <typename T, size_t row_, size_t col_, bool is_col_major_,
          template <typename, typename> typename... ext_methods,
          template <typename, typename> typename... special_methods>
struct base_of_mat_impl_helper<T, row_, col_, is_col_major_,
                               method_recorder<ext_methods...>,
                               method_recorder<special_methods...>>
    : return_type<std::conditional_t<
          is_col_major_,
          vec_impl<mat<T, row_, col_, is_col_major_>, vec<T, row_>, col_,
                   special_methods..., ext_methods...>,
          vec_impl<mat<T, row_, col_, is_col_major_>, vec<T, col_>, row_,
                   special_methods..., ext_methods...>>> {};

template <typename T, size_t row_, size_t col_, bool is_col_major_>
using base_of_mat_impl =
    details::dervef_type<base_of_mat_impl_helper<T, row_, col_, is_col_major_>>;
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
    mat_impl(const mat_impl&) = default;
    mat_impl(mat_impl&&) = default;
    mat_impl& operator=(const mat_impl&) = default;
    mat_impl& operator=(mat_impl&&) = default;

    constexpr decltype(auto) operator[](size_t i) {
        return static_cast<mat*>(this)->at(i);
    }
    constexpr decltype(auto) operator[](size_t i) const {
        return static_cast<const mat*>(this)->at(i);
    }
#if XCMATH_CXX_STD >= 202302L
    constexpr T& operator[](size_t i, size_t j) {
        return static_cast<mat*>(this)->at(i, j);
    }
    constexpr const T& operator[](size_t i, size_t j) const {
        return static_cast<const mat*>(this)->at(i, j);
    }
#endif
};

template <typename T, size_t row_, size_t col_>
class mat<T, row_, col_, false> : public mat_impl<T, row_, col_, false> {
    using impl = mat_impl<T, row_, col_, false>;
    using typename impl::Super;

   public:
    using impl::impl;

    constexpr mat& operator=(const mat<T, row_, col_, true>& other);
    using impl::at;
    inline constexpr T& at(size_t i, size_t j) { return Super::data_[i][j]; }
    inline constexpr const T& at(size_t i, size_t j) const {
        return Super::data_[i][j];
    }
};

template <typename T, size_t row_, size_t col_>
class mat<T, row_, col_, true> : public mat_impl<T, row_, col_, true> {
    using impl = mat_impl<T, row_, col_, true>;
    using typename impl::Super;
    using row_view = vec_view<T, col_, row_>;
    using const_row_view = const_vec_view<T, col_, row_>;

   public:
    constexpr mat() = default;
    constexpr mat(const mat&) = default;
    constexpr mat(mat&&) = default;
    mat& operator=(const mat&) = default;
    mat& operator=(mat&&) = default;
    constexpr mat(const std::initializer_list<vec<T, col_>>& init) : impl{} {
        size_t i = 0;
        for (const auto& row : init) {
            for (size_t j = 0; j < col_; ++j) {
                Super::data_[j][i] = row[j];
            }
            ++i;
        }
    }

    constexpr row_view at(size_t idx) {
        assert_index(idx, row_);
        return row_view(&at(idx, 0));
    }
    constexpr const_row_view at(size_t idx) const {
        assert_index(idx, row_);
        return const_row_view(&at(idx, 0));
    }
    constexpr T& at(size_t i, size_t j) { return Super::data_[j][i]; }
    constexpr const T& at(size_t i, size_t j) const {
        return Super::data_[j][i];
    }
    constexpr mat& operator=(const mat<T, row_, col_, false>& other) {
        for (size_t i = 0; i < row_; ++i) {
            for (size_t j = 0; j < col_; ++j) {
                this->data_[j][i] = other.at(i, j);
            }
        }
        return *this;
    }
};

// determinant_method specialization for 2x2 matrix
IMPL_METHOD_BEGIN(determinant_method, typename T, bool is_col_major_)
IMPL_METHOD_FOR(mat<T, 2, 2, is_col_major_>)
inline constexpr T determinant() const noexcept {
    const auto& self = *static_cast<const mat<T, 2, 2, is_col_major_>*>(this);
    return self.at(0, 0) * self.at(1, 1) - self.at(0, 1) * self.at(1, 0);
}
IMPL_METHOD_END()

// determinant_method specialization for 3x3 matrix
IMPL_METHOD_BEGIN(determinant_method, typename T, bool is_col_major_)
IMPL_METHOD_FOR(mat<T, 3, 3, is_col_major_>)
inline constexpr T determinant() const noexcept {
    const auto& self = *static_cast<const mat<T, 3, 3, is_col_major_>*>(this);
    return self.at(0, 0) *
               (self.at(1, 1) * self.at(2, 2) - self.at(1, 2) * self.at(2, 1)) -
           self.at(0, 1) *
               (self.at(1, 0) * self.at(2, 2) - self.at(1, 2) * self.at(2, 0)) +
           self.at(0, 2) *
               (self.at(1, 0) * self.at(2, 1) - self.at(1, 1) * self.at(2, 0));
}
IMPL_METHOD_END()

// determinant_method specialization for 4x4 matrix
IMPL_METHOD_BEGIN(determinant_method, typename T, bool is_col_major_)
IMPL_METHOD_FOR(mat<T, 4, 4, is_col_major_>)
inline constexpr T determinant() const noexcept {
    const auto& self = *static_cast<const mat<T, 4, 4, is_col_major_>*>(this);
    return self.at(0, 0) * (self.at(1, 1) * (self.at(2, 2) * self.at(3, 3) -
                                             self.at(2, 3) * self.at(3, 2)) -
                            self.at(1, 2) * (self.at(2, 1) * self.at(3, 3) -
                                             self.at(2, 3) * self.at(3, 1)) +
                            self.at(1, 3) * (self.at(2, 1) * self.at(3, 2) -
                                             self.at(2, 2) * self.at(3, 1))) -
           self.at(0, 1) * (self.at(1, 0) * (self.at(2, 2) * self.at(3, 3) -
                                             self.at(2, 3) * self.at(3, 2)) -
                            self.at(1, 2) * (self.at(2, 0) * self.at(3, 3) -
                                             self.at(2, 3) * self.at(3, 0)) +
                            self.at(1, 3) * (self.at(2, 0) * self.at(3, 2) -
                                             self.at(2, 2) * self.at(3, 0))) +
           self.at(0, 2) * (self.at(1, 0) * (self.at(2, 1) * self.at(3, 3) -
                                             self.at(2, 3) * self.at(3, 1)) -
                            self.at(1, 1) * (self.at(2, 0) * self.at(3, 3) -
                                             self.at(2, 3) * self.at(3, 0)) +
                            self.at(1, 3) * (self.at(2, 0) * self.at(3, 1) -
                                             self.at(2, 1) * self.at(3, 0))) -
           self.at(0, 3) * (self.at(1, 0) * (self.at(2, 1) * self.at(3, 2) -
                                             self.at(2, 2) * self.at(3, 1)) -
                            self.at(1, 1) * (self.at(2, 0) * self.at(3, 2) -
                                             self.at(2, 2) * self.at(3, 0)) +
                            self.at(1, 2) * (self.at(2, 0) * self.at(3, 1) -
                                             self.at(2, 1) * self.at(3, 0)));
}
IMPL_METHOD_END()

// determinant_method specialization for generic NxN matrix (Gaussian
// elimination)
IMPL_METHOD_BEGIN(determinant_method, typename T, size_t size_,
                  bool is_col_major_)
IMPL_METHOD_FOR(mat<T, size_, size_, is_col_major_>)
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
            if (xcmath::fabs(a.at(r, i)) > xcmath::fabs(a.at(pivot, i))) {
                pivot = r;
            }
        }

        // If pivot is zero, matrix is singular
        if (a.at(pivot, i) == number_meta::number_properties<T>::zero) {
            return number_meta::number_properties<T>::zero;
        }

        // Swap rows if needed
        if (pivot != i) {
            for (size_t c = i; c < size_; ++c) {
                std::swap(a.at(i, c), a.at(pivot, c));
            }
            det = -det;  // Row swap changes sign
        }

        // Elimination
        for (size_t r = i + 1; r < size_; ++r) {
            if (a.at(r, i) == number_meta::number_properties<T>::zero) continue;
            T factor = a.at(r, i) / a.at(i, i);
            for (size_t c = i; c < size_; ++c) {
                a.at(r, c) -= factor * a.at(i, c);
            }
        }
    }

    // Calculate product of diagonal
    for (size_t i = 0; i < size_; ++i) {
        det *= a.at(i, i);
    }

    return det;
}
IMPL_METHOD_END()
IMPL_FACTORY_BEGIN(impl_from_type_to_zero_method, typename T, size_t col_,
                   size_t row_, bool is_col_major_)
IMPL_FACTORY_FOR(mat<T, col_, row_, is_col_major_>)
template <typename Tp>
static inline constexpr auto impl_from_type_to_zero() noexcept {
    return number_meta::number_properties<
        mat<Tp, col_, row_, is_col_major_>>::zero;
}
IMPL_FACTORY_END()
namespace number_meta {
template <typename T, size_t row_, size_t col_, bool is_col_major_>
struct number_properties<mat<T, row_, col_, is_col_major_>> {
    using number_type = mat<T, row_, col_, is_col_major_>;
    static constexpr number_type zero{};
    static constexpr number_type unit = []() {
        number_type unit = zero;
        for (size_t i = 0; i < (col_ < row_ ? col_ : row_); ++i) {
            unit.at(i, i) = number_properties<T>::unit;
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
            this->data_[i][j] = other.at(i, j);
        }
    }
    return *this;
}

}  // namespace xcmath
