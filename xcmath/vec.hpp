#pragma once
#include <cstddef>
#include <initializer_list>
#include <type_traits>

#include "./alias.hpp"
#include "./methods.hpp"
#include "./point_accesser.hpp"
#include "number_meta.hpp"

namespace xcmath {
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
    : impl_methods_helper<vec_view<T, size_, stride_>, point_accesser> {
    constexpr const_vec_view(const T* data) : ptr_(data) {}
    constexpr const T& operator[](size_t idx) const {
        assert_index(idx, size_);
        return ptr_[idx * stride_];
    }

   protected:
    const T* ptr_;
};
template <typename T, size_t size_, size_t stride_>
constexpr size_t methods_size<const_vec_view<T, size_, stride_>>{size_};
template <typename T, size_t size_, size_t stride_>
constexpr size_t methods_size<vec_view<T, size_, stride_>>{size_};

template <typename T, size_t size_, size_t stride_>
struct vec_view
    : impl_methods_helper<vec_view<T, size_, stride_>, point_accesser> {
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
template <typename Derived, typename T, size_t size_>
class vec_impl;
template <typename Derived, typename T, size_t size_>
constexpr size_t methods_size<vec_impl<Derived, T, size_>>{size_};
template <typename T, size_t size_>
constexpr size_t methods_size<vec<T, size_>>{size_};
template <typename Derived, typename T, size_t size_>
class vec_impl : public impl_methods<Derived, point_accesser,
                                     zero_factory_method, unit_factory_method,
                                     module_method, normalize_method> {
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
    constexpr vec_impl(std::initializer_list<item_type> init_list) {
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

namespace number_meta {
template <typename T, size_t size_>
struct number_properties<vec<T, size_>> {
    using number_type = vec<T, size_>;
    static constexpr number_type zero = number_type{number_properties<T>::zero};
    static constexpr number_type unit = number_type{number_properties<T>::unit};
};
}  // namespace number_meta

}  // namespace xcmath