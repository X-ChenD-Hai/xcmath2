#pragma once
#include <initializer_list>
#include <type_traits>

#include "./alias.hpp"
#include "./vec.hpp"

namespace xcmath {
template <typename T, size_t row_, size_t col_, bool is_col_major_>
struct mat_impl
    : public std::conditional_t<
          is_col_major_,
          vec_impl<mat<T, row_, col_, is_col_major_>, vec<T, row_>, col_>,
          vec_impl<mat<T, row_, col_, is_col_major_>, vec<T, col_>, row_>> {
   private:
    using mat = mat<T, row_, col_, is_col_major_>;

   protected:
    using Super =
        std::conditional_t<is_col_major_, vec_impl<mat, vec<T, row_>, col_>,
                           vec_impl<mat, vec<T, col_>, row_>>;

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