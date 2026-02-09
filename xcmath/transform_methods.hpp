#pragma once
#include <cstddef>
#include <type_traits>

#include "./alias.hpp"
#include "./functions.hpp"
#include "./mat_methods.hpp"
#include "./methods.hpp"
#include "./traits.hpp"

#define self (*static_cast<Self*>(this))
#define const_self (*static_cast<const Self*>(this))
#define require_method(method)                  \
    static_assert(is_impl_method<Self, method>, \
                  "Derived must be derived from " #method)

namespace xcmath {

IMPL_METHOD_BEGIN_WITH_REQUIRES(rotate_method,
                                (traits::value_in_range<size_, 2, 4>),
                                typename T, size_t size_, bool is_col_major_)
IMPL_METHOD_FOR(mat<T, size_, size_, is_col_major_>)
template <typename ATp, typename VTp>
    requires(traits::length_in_range<VTp, 3, 4> &&
             std::is_floating_point_v<ATp> &&
             traits::value_in_range<size_, 3, 4>)
auto rotate(const ATp angle, const VTp& axis) const noexcept {
    auto _axis = axis.normalize();
    const auto r_angle = xcmath::radians(angle);
    const auto c = cos(r_angle);
    const auto s = sin(r_angle);
    const auto t = 1 - c;
    auto res = Self::unit();
    res.at(0, 0) = c + t * _axis[0] * _axis[0];
    res.at(0, 1) = t * _axis[0] * _axis[1] - s * _axis[2];
    res.at(0, 2) = t * _axis[0] * _axis[2] + s * _axis[1];

    res.at(1, 0) = t * _axis[0] * _axis[1] + s * _axis[2];
    res.at(1, 1) = c + t * _axis[1] * _axis[1];
    res.at(1, 2) = t * _axis[1] * _axis[2] - s * _axis[0];

    res.at(2, 0) = t * _axis[0] * _axis[2] - s * _axis[1];
    res.at(2, 1) = t * _axis[1] * _axis[2] + s * _axis[0];
    res.at(2, 2) = c + t * _axis[2] * _axis[2];
    return res * const_self;
}
template <typename ATp>
    requires(std::is_floating_point_v<ATp> &&
             traits::value_in_range<size_, 2, 3>)
auto rotate(ATp angle) {
    angle = radians(angle);
    auto res = Self::unit();
    res.at(1, 1) = res.at(0, 0) = xcmath::cos(angle);
    res.at(0, 1) = -(res.at(1, 0) = xcmath::sin(angle));
    return res * const_self;
}

IMPL_METHOD_END();

IMPL_METHOD_BEGIN_WITH_REQUIRES(translate_method,
                                (traits::value_in_range<size_, 3, 4>),
                                typename T, size_t size_, bool is_col_major_)
IMPL_METHOD_FOR(mat<T, size_, size_, is_col_major_>)
template <class VTp>
    requires(traits::length_in_range<VTp, size_ - 1, size_>)
constexpr auto translate(const VTp& v) {
    auto res = const_self.clone();
    res.at(0, size_ - 1) += v[0];
    res.at(1, size_ - 1) += v[1];
    if constexpr (size_ == 4) res.at(2, size_ - 1) += v[2];
    return res;
}
IMPL_METHOD_END();

IMPL_METHOD_BEGIN_WITH_REQUIRES(scale_method,
                                (traits::value_in_range<size_, 3, 4>),
                                typename T, size_t size_, bool is_col_major_)
IMPL_METHOD_FOR(mat<T, size_, size_, is_col_major_>)
template <class VTp>
    requires(traits::length_in_range<VTp, 3, 4>)
constexpr auto scale(const VTp& v) {
    auto res = const_self.clone();
    res.at(0, 0) *= v[0];
    res.at(1, 1) *= v[1];
    if constexpr (size_ == 4) res.at(2, 2) *= v[2];
    return res;
}
template <class VTp>
    requires(std::is_floating_point_v<VTp>)
constexpr auto scale(VTp s) {
    auto res = const_self.clone();
    res.at(0, 0) *= s;
    res.at(1, 1) *= s;
    if constexpr (size_ == 4) res.at(2, 2) *= s;
    return res;
}
IMPL_METHOD_END();

}  // namespace xcmath

#undef self
#undef const_self
#undef require_method
