#pragma once
#include <cstddef>
#include <type_traits>

#include "./alias.hpp"
#include "xcmixin/xcmixin.hpp"
namespace xcmath {

namespace details {
template <typename recorder>
struct return_type {
    using type = recorder;
};
template <typename T>
using dervef_type = typename T::type;
}  // namespace details

namespace traits {

template <size_t idx, auto v0, auto... value>
    requires(idx <= sizeof...(value))
static constexpr auto value_at = value_at<idx - 1, value...>;
template <auto v0, auto... value>
static constexpr auto value_at<0, v0, value...> = v0;
template <auto... values>
struct value_all_equal {
    template <auto... ovalue>
    static constexpr bool value = ((values == ovalue) && ...);
    template <auto... ovalue>
        requires(sizeof...(ovalue) != sizeof...(values))
    static constexpr bool value<ovalue...> = false;
};

template <size_t... length_>
struct shape_recorder {
    template <size_t olength_>
    using push_back = shape_recorder<length_..., olength_>;
    template <size_t olength_>
    using push_front = shape_recorder<olength_, length_...>;
    template <size_t idx>
    static constexpr size_t length_at = value_at<idx, length_...>;
    template <typename oshape_recorder>
    static constexpr bool is_same_shape = false;
    template <size_t... olengths_>
    static constexpr bool is_same_shape<shape_recorder<olengths_...>> =
        value_all_equal<length_...>::template value<olengths_...>;
};

template <typename T>
struct shape_properties {
    using element_type = T;
    using shape_recorder = ::xcmath::traits::shape_recorder<>;
    static constexpr size_t length = 0;
    static constexpr size_t dim = 0;
    static constexpr bool is_dynamic = false;
};

template <typename T>
struct shape_properties<T&> : shape_properties<T> {};
template <typename T>
struct shape_properties<const T> : shape_properties<T> {};
template <typename T>
struct shape_properties<volatile T> : shape_properties<T> {};

template <typename element_type_, size_t length_,
          size_t dim_ = shape_properties<element_type_>::dim + 1,
          typename shape_recorder_ = shape_properties<
              element_type_>::shape_recorder::template push_front<length_>>
struct shape_properties_helper {
    using element_type = element_type_;
    using shape_recorder = shape_recorder_;
    static constexpr size_t length = length_;
    static constexpr size_t dim = dim_;
    static constexpr bool is_dynamic = false;
};
template <typename T>
using element_type = typename shape_properties<T>::element_type;

template <typename T, typename U>
static constexpr bool is_same_shape =
    shape_properties<T>::shape_recorder::template is_same_shape<
        typename shape_properties<U>::shape_recorder>;
template <typename T, typename U>
static constexpr bool is_same_dim =
    shape_properties<T>::dim == shape_properties<U>::dim;
template <typename T, typename U>
static constexpr bool is_same_length =
    shape_properties<T>::length == shape_properties<U>::length;
template <typename T>
static constexpr bool is_scal = shape_properties<T>::dim == 0;
template <typename T>
static constexpr bool is_vec = !is_scal<T>;
template <typename T>
static constexpr bool is_mat = false;
template <typename T, size_t row_, size_t col_, bool is_col_major_>
static constexpr bool is_mat<mat<T, row_, col_, is_col_major_>> = true;
template <typename T>
static constexpr bool is_pure_vec = shape_properties<T>::dim == 1;
template <typename T>
static constexpr bool is_pure_mat = is_mat<T> && is_pure_vec<element_type<T>>;

template <typename T, typename U>
constexpr bool length_eq =
    shape_properties<T>::length == shape_properties<U>::length;
template <typename T, typename U>
constexpr bool length_gt =
    shape_properties<T>::length > shape_properties<U>::length;
template <typename T, typename U>
constexpr bool length_ge =
    shape_properties<T>::length >= shape_properties<U>::length;
template <typename T, typename U>
constexpr bool length_lt =
    shape_properties<T>::length < shape_properties<U>::length;
template <typename T, typename U>
constexpr bool length_le =
    shape_properties<T>::length <= shape_properties<U>::length;
template <auto number_, auto min_, auto max_>
constexpr bool value_in_range = number_ <= max_ && number_ >= min_;
template <typename T, size_t min_, size_t max_>
constexpr bool length_in_range =
    value_in_range<shape_properties<T>::length, min_, max_>;

// Helper traits to extract matrix dimensions from the method recorder chain
template <typename T>
struct mat_dims_from_base {
    static constexpr size_t rows = 0;
    static constexpr size_t cols = 0;
    static constexpr bool is_col_major = false;
};

// Specialization that extracts dims from any type
template <typename T>
struct mat_dims : mat_dims_from_base<T> {};
// Specialize mat_dims for all matrix types
template <typename T, size_t row_, size_t col_, bool is_col_major_>
struct mat_dims<mat<T, row_, col_, is_col_major_>> {
    static constexpr size_t rows = row_;
    static constexpr size_t cols = col_;
    static constexpr bool is_col_major = is_col_major_;
};
// traits
template <typename T, size_t size_>
struct shape_properties<vec<T, size_>> : shape_properties_helper<T, size_> {};

template <typename T, size_t size_, size_t stride_>
struct shape_properties<vec_view<T, size_, stride_>>
    : shape_properties_helper<T, size_> {};

template <typename T, size_t size_, size_t stride_>
struct shape_properties<const_vec_view<T, size_, stride_>>
    : shape_properties_helper<T, size_> {};

template <typename T, size_t row_, size_t col_, bool is_col_major_>
struct shape_properties<mat<T, row_, col_, is_col_major_>>
    : shape_properties_helper<
          std::conditional_t<is_col_major_, vec_view<T, col_, row_>,
                             vec<T, col_>>,
          row_, 2, shape_recorder<row_, col_>> {};

}  // namespace traits

template <typename T, size_t row_, size_t col_, bool is_col_major_>
struct special_mat_ext_methods_recorder;

template <template <typename, typename> typename... methods>
using impl_special_mat_ext_methods =
    details::return_type<xcmixin::method_recorder<methods...>>;
}  // namespace xcmath