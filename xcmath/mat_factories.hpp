#pragma once
#include "./functions.hpp"
#include "./mat_methods.hpp"

// open scope
#include "xcmixin/scope_open.hpp"
namespace xcmath {
IMPL_FACTORY_BEGIN(look_at_factory, typename T, bool is_col_major)
IMPL_FACTORY_FOR(mat<T, 4, 4, is_col_major>)
template <typename Tp>
static constexpr auto look_at(const Tp& eye, const Tp& center, const Tp& up) {
    auto result = Self::unit();
    const auto forward = (center - eye).normalize();
    const auto right = up.cross(forward).normalize();
    const auto new_up = forward.cross(right);
    result.at(0, 0) = right[0];
    result.at(0, 1) = right[1];
    result.at(0, 2) = right[2];
    result.at(1, 0) = new_up[0];
    result.at(1, 1) = new_up[1];
    result.at(1, 2) = new_up[2];
    result.at(2, 0) = -forward[0];
    result.at(2, 1) = -forward[1];
    result.at(2, 2) = -forward[2];
    return result;
}
IMPL_FACTORY_END();

IMPL_FACTORY_BEGIN(perspective_factory, typename T, bool is_col_major)
IMPL_FACTORY_FOR(mat<T, 4, 4, is_col_major>)
template <typename Tp>
static constexpr auto perspective(Tp fov, Tp aspect, Tp near, Tp far) {
    auto result = Self::unit();
    const float fov_rad = xcmath::radians(fov);
    const float tan_half = xcmath::tan(fov_rad * 0.5f);
    const float n = near;
    const float f = far;
    const float a = aspect;

    result.at(0, 0) = 1.0f / (a * tan_half);
    result.at(1, 1) = 1.0f / tan_half;
    result.at(2, 2) = -(f + n) / (f - n);
    result.at(2, 3) = -2.0f * f * n / (f - n);
    result.at(3, 2) = -1.0f;
}
FACTORY_DEF_END()

}  // namespace xcmath
#include "xcmixin/scope_close.hpp"