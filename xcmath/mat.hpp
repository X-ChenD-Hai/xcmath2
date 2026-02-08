#pragma once
#include <initializer_list>
#include <type_traits>

#include "./alias.hpp"
#include "./mat_methods.hpp"
#include "./vec.hpp"
#include "methods.hpp"

namespace xcmath {
using comman_mat_ext_methods =
    method_recorder<trace_method, transpose_method, inverse_method>;
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