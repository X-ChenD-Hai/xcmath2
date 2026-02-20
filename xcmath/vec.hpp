#pragma once
#include <cstddef>
#include <initializer_list>
#include <type_traits>

#include "./alias.hpp"
#include "./factories.hpp"
#include "./methods.hpp"
#include "./number_meta.hpp"
#include "./operators.hpp"
#include "./point_accesser.hpp"  // IWYU pragma: keep
#include "xcmixin/scope_open.hpp"
#include "xcmixin/xcmixin.hpp"
namespace xcmath {
template <typename T, size_t size_>
using vec_impl_methods = xcmixin::recorder_concat<
    xcmixin::method_recorder<point_accesser_sized<size_>::template type>,
    vec_factory_methods_recorder, vec_member_methods_recorder,
    vec_double_operator_methods_recorder, vec_single_operator_methods_recorder>;

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

template <typename Derived, typename T, size_t size_, typename... ext_recorders>
class vec_impl
    : public xcmixin::impl_methods_recorders<Derived, ext_recorders...,
                                             vec_impl_methods<T, size_>> {
   public:
    using item_type = T;
    using data_type = vec_properties<T>::data_type;

   public:
    constexpr vec_impl() = default;
    vec_impl(const vec_impl&) = default;
    vec_impl(vec_impl&&) = default;
    vec_impl& operator=(const vec_impl&) = default;
    vec_impl& operator=(vec_impl&&) = default;
    explicit constexpr vec_impl(const T& fill_data) : data_{fill_data} {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = fill_data;
        }
    }
    constexpr vec_impl(const std::initializer_list<item_type>& init_list) {
        assert_index(init_list.size(), size_ + 1);
        size_t i = 0;
        for (auto item : init_list) {
            data_[i++] = item;
        }
    }

    constexpr T& at(size_t idx) {
        assert_index(idx, size_);
        return data_[idx];
    }
    constexpr const T& at(size_t idx) const {
        assert_index(idx, size_);
        return data_[idx];
    }
    constexpr T& operator[](size_t idx) { return at(idx); }
    constexpr const T& operator[](size_t idx) const { return at(idx); }

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

template <typename T, size_t size_, size_t stride_>
struct const_vec_view
    : xcmixin::impl_methods_recorders<const_vec_view<T, size_, stride_>,
                                      vec_impl_methods<T, size_>> {
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
struct vec_view : xcmixin::impl_methods_recorders<vec_view<T, size_, stride_>,
                                                  vec_impl_methods<T, size_>> {
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
IMPL_METHOD_FOR(vec_view<T, size_, stride_>)
inline constexpr auto clone() const noexcept {
    vec<T, size_> result;
    for (size_t i = 0; i < size_; ++i) {
        result[i] = const_self[i];
    }
    return result;
}
IMPL_METHOD_END()

IMPL_METHOD_BEGIN(clone_method, typename T, size_t size_, size_t stride_)
IMPL_METHOD_FOR(const_vec_view<T, size_, stride_>)
inline constexpr auto clone() const noexcept {
    vec<T, size_> result;
    for (size_t i = 0; i < size_; ++i) {
        result[i] = const_self[i];
    }
    return result;
}
IMPL_METHOD_END()

IMPL_FACTORY_BEGIN(impl_from_type_to_zero_factory, typename T, size_t size_)
IMPL_FACTORY_FOR(vec<T, size_>)
template <typename Tp>
static inline constexpr auto impl_from_type_to_zero() noexcept {
    return number_meta::number_properties<vec<Tp, size_>>::zero;
}
IMPL_FACTORY_END()
IMPL_FACTORY_BEGIN(impl_from_type_to_zero_factory, typename T, size_t size_,
                   size_t stride_)
IMPL_FACTORY_FOR(vec_view<T, size_, stride_>)
template <typename Tp>
static inline constexpr auto impl_from_type_to_zero() noexcept {
    return number_meta::number_properties<vec<Tp, size_>>::zero;
}
IMPL_FACTORY_END()
IMPL_FACTORY_BEGIN(impl_from_type_to_zero_factory, typename T, size_t size_,
                   size_t stride_)
IMPL_FACTORY_FOR(const_vec_view<T, size_, stride_>)
template <typename Tp>
static inline constexpr auto impl_from_type_to_zero() noexcept {
    return number_meta::number_properties<vec<Tp, size_>>::zero;
}
IMPL_FACTORY_END()

template <typename T, size_t size_>
class vec : public vec_impl<vec<T, size_>, T, size_> {
   public:
    using vec_impl<vec<T, size_>, T, size_>::vec_impl;
};

IMPL_METHOD_BEGIN(unit_factory, typename T, size_t size_)
IMPL_METHOD_FOR(vec<T, size_>)
inline static constexpr auto unit() noexcept {
    static_assert(false, "vec not supported unit_factory");
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

#include "xcmixin/scope_close.hpp"