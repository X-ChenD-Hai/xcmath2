#pragma once
#include <cstddef>
#include <initializer_list>
#include <type_traits>

#include "./alias.hpp"
#include "./methods.hpp"
#include "./point_accesser.hpp"  // IWYU pragma: keep
#include "number_meta.hpp"

namespace xcmath {
template <typename Derived, typename T, size_t size_>
using vec_impl_methods = method_recorder<
    point_accesser_sized<size_>::template type, size_method, clone_method,
    move_method, zero_factory_method, unit_factory_method, module_method,
    normalize_method, dot_method, distance_method, distance_squared_method,
    angle_method, project_method, reflect_method, refract_method,
    cross_product_method, abs_method, min_method, max_method, clamp_method,
    floor_method, ceil_method, round_method, fract_method, sign_method,
    equal_method, less_than_method, greater_than_method, any_method,
    all_method>;
namespace details {
template <typename Derived, typename T, size_t size_, typename method_recorder,
          template <typename, typename> class... ext_methods>
struct base_of_vec_impl_helper;
template <typename Derived, typename T, size_t size_,
          template <typename, typename> class... methods,
          template <typename, typename> class... ext_methods>
struct base_of_vec_impl_helper<Derived, T, size_, method_recorder<methods...>,
                               ext_methods...> {
    using type = impl_methods<Derived, ext_methods..., methods...>;
};

template <typename Derived, typename T, size_t size_,
          template <typename, typename> class... ext_methods>
using base_of_vec_impl =
    base_of_vec_impl_helper<Derived, T, size_,
                            vec_impl_methods<Derived, T, size_>,
                            ext_methods...>::type;
}  // namespace details

template <typename T, typename = void>
struct vec_properties {
    static constexpr size_t dim = 0;
    using data_type = T;
};
template <typename T>
struct vec_properties<T, std::void_t<typename T::data_type>> {
    static constexpr size_t dim =
        1 + vec_properties<typename T::item_type>::dim;
    using data_type = vec_properties<typename T::item_type>::data_type;
};
template <typename T, size_t size_, size_t stride_>
struct const_vec_view
    : details::base_of_vec_impl<const_vec_view<T, size_, stride_>, T, size_> {
    static inline constexpr size_t size() noexcept { return size_; }

    constexpr const_vec_view(const T* data) : ptr_(data) {}
    constexpr const T& operator[](size_t idx) const {
        assert_index(idx, size_);
        return ptr_[idx * stride_];
    }

   protected:
    const T* ptr_;
};

template <typename T, size_t size_, size_t stride_>
struct vec_view
    : details::base_of_vec_impl<const_vec_view<T, size_, stride_>, T, size_> {
    static inline constexpr size_t size() noexcept { return size_; }

    constexpr vec_view(T* data) : ptr_(data) {}
    constexpr T& operator[](size_t idx) {
        assert_index(idx, size_);
        return ptr_[idx * stride_];
    }
    constexpr const T& operator[](size_t idx) const {
        assert_index(idx, size_);
        return ptr_[idx * stride_];
    }

    constexpr operator const_vec_view<T, size_, stride_>() const {
        return const_vec_view<T, size_, stride_>(ptr_);
    }
    template <size_t ostride_>
    constexpr vec_view operator=(const_vec_view<T, size_, ostride_> v) {
        for (size_t i = 0; i < size_; ++i) {
            ptr_[i * stride_] = v[i];
        }
        return *this;
    }

   protected:
    T* ptr_;
};

IMPL_METHOD_BEGIN(clone_method, typename T, size_t size_, size_t stride_)
IMPL_METHOD_FOR(const_vec_view<T, size_, stride_>)
inline constexpr auto clone() const noexcept {
    using Self = const const_vec_view<T, size_, stride_>;
    vec<T, size_> result;
    for (size_t i = 0; i < size_; ++i) {
        result[i] = static_cast<Self*>(this)->operator[](i);
    }
    return result;
}
IMPL_METHOD_END()

IMPL_METHOD_BEGIN(clamp_method, typename T, size_t size_, size_t stride_)
IMPL_METHOD_FOR(vec_view<T, size_, stride_>)
inline constexpr auto clone() const noexcept {
    using Self = const vec_view<T, size_, stride_>;
    vec<T, size_> result;
    for (size_t i = 0; i < size_; ++i) {
        result[i] = static_cast<Self*>(this)->operator[](i);
    }
    return result;
}
IMPL_METHOD_END()

template <typename Derived, typename T, size_t size_,
          template <typename, typename> class... ext_methods>
class vec_impl
    : public details::base_of_vec_impl<Derived, T, size_, ext_methods...> {
   public:
    using item_type = T;
    using data_type = vec_properties<T>::data_type;

   public:
    explicit constexpr vec_impl(const T& fill_data) : data_{fill_data} {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = fill_data;
        }
    }
    constexpr vec_impl() : data_{} {}
    constexpr vec_impl(const std::initializer_list<item_type>& init_list) {
        assert_index(init_list.size(), size_ + 1);
        size_t i = 0;
        for (auto item : init_list) {
            data_[i++] = item;
        }
    }

    constexpr T& operator[](size_t idx) { return data_[idx]; }
    constexpr const T& operator[](size_t idx) const { return data_[idx]; }

    constexpr operator vec_view<T, size_, 1>() {
        return vec_view<T, size_, 1>(data_);
    }
    constexpr operator const_vec_view<T, size_, 1>() const {
        return const_vec_view<T, size_, 1>(data_);
    }

    data_type* raw_ptr() { return reinterpret_cast<data_type*>(data_); }
    const data_type* raw_ptr() const {
        return reinterpret_cast<const data_type*>(data_);
    }

   protected:
    T data_[size_]{number_meta::number_properties<T>::zero};
};

template <typename T, size_t size_>
class vec : public vec_impl<vec<T, size_>, T, size_> {
   public:
    using vec_impl<vec<T, size_>, T, size_>::vec_impl;
};

IMPL_METHOD_BEGIN(unit_factory_method, typename T, size_t size_)
IMPL_METHOD_FOR(vec<T, size_>)
inline static constexpr size_t unit() noexcept {
    static_assert(false, "vec not supported unit_factory_method");
}
IMPL_METHOD_END()

namespace number_meta {
template <typename T, size_t size_>
struct number_properties<vec<T, size_>> {
    using number_type = vec<T, size_>;
    static constexpr number_type zero = number_type(number_properties<T>::zero);
};
}  // namespace number_meta

}  // namespace xcmath