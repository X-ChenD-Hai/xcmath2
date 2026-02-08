#pragma once
#include <cstddef>

#include "./alias.hpp"

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

template <typename T>
struct length_properties {
    static constexpr size_t length = 0;
    static constexpr bool is_dynamic = false;
};

template <typename T>
struct length_properties<T&> : length_properties<T> {};
template <typename T>
struct length_properties<const T> : length_properties<T> {};
template <typename T>
struct length_properties<volatile T> : length_properties<T> {};

template <size_t length_>
struct length_properties_helper {
    static constexpr size_t length = length_;
    static constexpr bool is_dynamic = false;
};

template <typename T, typename U>
constexpr bool length_eq =
    length_properties<T>::length == length_properties<U>::length;
template <typename T, typename U>
constexpr bool length_gt =
    length_properties<T>::length > length_properties<U>::length;
template <typename T, typename U>
constexpr bool length_ge =
    length_properties<T>::length >= length_properties<U>::length;
template <typename T, typename U>
constexpr bool length_lt =
    length_properties<T>::length < length_properties<U>::length;
template <typename T, typename U>
constexpr bool length_le =
    length_properties<T>::length <= length_properties<U>::length;
template <auto number_, auto min_, auto max_>
constexpr bool value_in_range = number_ <= max_ && number_ >= min_;
template <typename T, size_t min_, size_t max_>
constexpr bool length_in_range =
    value_in_range<length_properties<T>::length, min_, max_>;

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
struct length_properties<vec<T, size_>> : length_properties_helper<size_> {};

template <typename T, size_t size_, size_t stride_>
struct length_properties<vec_view<T, size_, stride_>>
    : length_properties_helper<size_> {};

template <typename T, size_t size_, size_t stride_>
struct length_properties<const_vec_view<T, size_, stride_>>
    : length_properties_helper<size_> {};

template <typename T, size_t row_, size_t col_, bool is_dynamic_>
struct length_properties<mat<T, row_, col_, is_dynamic_>>
    : length_properties_helper<row_> {};

}  // namespace traits

template <template <typename, typename> class... methods>
struct method_recorder;
template <template <typename, typename> class method, typename recorder>
static constexpr bool has_method = false;

template <template <typename, typename> class method,
          template <typename, typename> class... methods>
static constexpr bool has_method<method, method_recorder<methods...>> =
    (std::is_same_v<method<void, void>, methods<void, void>> || ...);

template <typename Derived, template <typename, typename> class method>
static constexpr bool is_impl_method =
    has_method<method, typename Derived::method_recorder>;

template <typename T, size_t row_, size_t col_, bool is_col_major_>
struct spical_mat_ext_methods_recorder;

template <template <typename, typename> typename... methods>
using impl_spical_mat_ext_methods =
    details::return_type<method_recorder<methods...>>;
}  // namespace xcmath