#pragma once
#include <cstddef>
#include <type_traits>

#include "./alias.hpp"
#include "./functions.hpp"
#include "./mat_methods.hpp"
#include "./traits.hpp"

namespace xcmath {

XCMIXIN_IMPL_BEGIN_WITH_REQUIRES(rotate_method,
                                (traits::value_in_range<size_, 2, 4>),
                                typename T, size_t size_, bool is_col_major_)
XCMIXIN_IMPL_FOR(mat<T, size_, size_, is_col_major_>)
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
    return res * xcmixin_const_self;
}
template <typename ATp>
    requires(std::is_floating_point_v<ATp> &&
             traits::value_in_range<size_, 2, 3>)
auto rotate(ATp angle) {
    angle = radians(angle);
    auto res = Self::unit();
    res.at(1, 1) = res.at(0, 0) = xcmath::cos(angle);
    res.at(0, 1) = -(res.at(1, 0) = xcmath::sin(angle));
    return res * xcmixin_const_self;
}

XCMIXIN_IMPL_END();

XCMIXIN_IMPL_BEGIN_WITH_REQUIRES(rotate_x_method,
                                (traits::value_in_range<size_, 3, 4>),
                                typename T, size_t size_, bool is_col_major_)
XCMIXIN_IMPL_FOR(mat<T, size_, size_, is_col_major_>)
template <typename ATp>
    requires(std::is_floating_point_v<ATp>)
constexpr auto rotate_x(ATp angle) {
    angle = radians(angle);
    const auto c = xcmath::cos(angle);
    const auto s = xcmath::sin(angle);
    // Build rotation matrix
    mat<T, size_, size_, is_col_major_> rot;
    if constexpr (size_ == 3) {
        rot.at(0, 0) = 1;
        rot.at(0, 1) = 0;
        rot.at(0, 2) = 0;
        rot.at(1, 0) = 0;
        rot.at(1, 1) = c;
        rot.at(1, 2) = -s;
        rot.at(2, 0) = 0;
        rot.at(2, 1) = s;
        rot.at(2, 2) = c;
    } else {
        rot.at(0, 0) = 1;
        rot.at(0, 1) = 0;
        rot.at(0, 2) = 0;
        rot.at(0, 3) = 0;
        rot.at(1, 0) = 0;
        rot.at(1, 1) = c;
        rot.at(1, 2) = -s;
        rot.at(1, 3) = 0;
        rot.at(2, 0) = 0;
        rot.at(2, 1) = s;
        rot.at(2, 2) = c;
        rot.at(2, 3) = 0;
        rot.at(3, 0) = 0;
        rot.at(3, 1) = 0;
        rot.at(3, 2) = 0;
        rot.at(3, 3) = 1;
    }
    return rot * xcmixin_const_self;
}
XCMIXIN_IMPL_END();

XCMIXIN_IMPL_BEGIN_WITH_REQUIRES(rotate_y_method,
                                (traits::value_in_range<size_, 3, 4>),
                                typename T, size_t size_, bool is_col_major_)
XCMIXIN_IMPL_FOR(mat<T, size_, size_, is_col_major_>)
template <typename ATp>
    requires(std::is_floating_point_v<ATp>)
constexpr auto rotate_y(ATp angle) {
    angle = radians(angle);
    const auto c = xcmath::cos(angle);
    const auto s = xcmath::sin(angle);
    // Build rotation matrix
    mat<T, size_, size_, is_col_major_> rot;
    if constexpr (size_ == 3) {
        rot.at(0, 0) = c;
        rot.at(0, 1) = 0;
        rot.at(0, 2) = s;
        rot.at(1, 0) = 0;
        rot.at(1, 1) = 1;
        rot.at(1, 2) = 0;
        rot.at(2, 0) = -s;
        rot.at(2, 1) = 0;
        rot.at(2, 2) = c;
    } else {
        rot.at(0, 0) = c;
        rot.at(0, 1) = 0;
        rot.at(0, 2) = s;
        rot.at(0, 3) = 0;
        rot.at(1, 0) = 0;
        rot.at(1, 1) = 1;
        rot.at(1, 2) = 0;
        rot.at(1, 3) = 0;
        rot.at(2, 0) = -s;
        rot.at(2, 1) = 0;
        rot.at(2, 2) = c;
        rot.at(2, 3) = 0;
        rot.at(3, 0) = 0;
        rot.at(3, 1) = 0;
        rot.at(3, 2) = 0;
        rot.at(3, 3) = 1;
    }
    return rot * xcmixin_const_self;
}
XCMIXIN_IMPL_END();

XCMIXIN_IMPL_BEGIN_WITH_REQUIRES(rotate_z_method,
                                (traits::value_in_range<size_, 2, 4>),
                                typename T, size_t size_, bool is_col_major_)
XCMIXIN_IMPL_FOR(mat<T, size_, size_, is_col_major_>)
template <typename ATp>
    requires(std::is_floating_point_v<ATp>)
constexpr auto rotate_z(ATp angle) {
    angle = radians(angle);
    const auto c = xcmath::cos(angle);
    const auto s = xcmath::sin(angle);
    // Build rotation matrix
    mat<T, size_, size_, is_col_major_> rot;
    if constexpr (size_ == 2) {
        rot.at(0, 0) = c;
        rot.at(0, 1) = -s;
        rot.at(1, 0) = s;
        rot.at(1, 1) = c;
    } else if constexpr (size_ == 3) {
        rot.at(0, 0) = c;
        rot.at(0, 1) = -s;
        rot.at(0, 2) = 0;
        rot.at(1, 0) = s;
        rot.at(1, 1) = c;
        rot.at(1, 2) = 0;
        rot.at(2, 0) = 0;
        rot.at(2, 1) = 0;
        rot.at(2, 2) = 1;
    } else {
        rot.at(0, 0) = c;
        rot.at(0, 1) = -s;
        rot.at(0, 2) = 0;
        rot.at(0, 3) = 0;
        rot.at(1, 0) = s;
        rot.at(1, 1) = c;
        rot.at(1, 2) = 0;
        rot.at(1, 3) = 0;
        rot.at(2, 0) = 0;
        rot.at(2, 1) = 0;
        rot.at(2, 2) = 1;
        rot.at(2, 3) = 0;
        rot.at(3, 0) = 0;
        rot.at(3, 1) = 0;
        rot.at(3, 2) = 0;
        rot.at(3, 3) = 1;
    }
    return rot * xcmixin_const_self;
}
XCMIXIN_IMPL_END();

XCMIXIN_IMPL_BEGIN_WITH_REQUIRES(shear_method,
                                (traits::value_in_range<size_, 3, 4>),
                                typename T, size_t size_, bool is_col_major_)
XCMIXIN_IMPL_FOR(mat<T, size_, size_, is_col_major_>)
template <class VTp>
    requires(traits::length_in_range<VTp, 6, 6>)
constexpr auto shear(const VTp& factors) {
    auto res = xcmixin_const_self.clone();
    // factors = [sh_xy, sh_xz, sh_yx, sh_yz, sh_zx, sh_zy]
    res.at(0, 1) += factors[0];  // sh_xy
    res.at(0, 2) += factors[1];  // sh_xz
    res.at(1, 0) += factors[2];  // sh_yx
    res.at(1, 2) += factors[3];  // sh_yz
    res.at(2, 0) += factors[4];  // sh_zx
    res.at(2, 1) += factors[5];  // sh_zy
    return res;
}
XCMIXIN_IMPL_END();

XCMIXIN_IMPL_BEGIN_WITH_REQUIRES(translate_method,
                                (traits::value_in_range<size_, 3, 4>),
                                typename T, size_t size_, bool is_col_major_)
XCMIXIN_IMPL_FOR(mat<T, size_, size_, is_col_major_>)
template <class VTp>
    requires(traits::length_in_range<VTp, size_ - 1, size_>)
constexpr auto translate(const VTp& v) {
    auto res = xcmixin_const_self.clone();
    res.at(0, size_ - 1) += v[0];
    res.at(1, size_ - 1) += v[1];
    if constexpr (size_ == 4) res.at(2, size_ - 1) += v[2];
    return res;
}
XCMIXIN_IMPL_END();

XCMIXIN_IMPL_BEGIN_WITH_REQUIRES(scale_method,
                                (traits::value_in_range<size_, 3, 4>),
                                typename T, size_t size_, bool is_col_major_)
XCMIXIN_IMPL_FOR(mat<T, size_, size_, is_col_major_>)
template <class VTp>
    requires(traits::length_in_range<VTp, 3, 4>)
constexpr auto scale(const VTp& v) {
    auto res = xcmixin_const_self.clone();
    res.at(0, 0) *= v[0];
    res.at(1, 1) *= v[1];
    if constexpr (size_ == 4) res.at(2, 2) *= v[2];
    return res;
}
template <class VTp>
    requires(std::is_floating_point_v<VTp>)
constexpr auto scale(VTp s) {
    auto res = xcmixin_const_self.clone();
    res.at(0, 0) *= s;
    res.at(1, 1) *= s;
    if constexpr (size_ == 4) res.at(2, 2) *= s;
    return res;
}
XCMIXIN_IMPL_END();

}  // namespace xcmath

