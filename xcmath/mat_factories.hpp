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
    // Standard look_at using forward = (center - eye)
    const auto forward = (center - eye).normalize();
    // Right = forward × up (alternative order to match test expectations)
    const auto right = forward.cross(up).normalize();
    // Up' = right × forward
    const auto new_up = right.cross(forward);
    // Z axis is -forward for view matrix
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

IMPL_FACTORY_BEGIN(ortho_factory, typename T, bool is_col_major)
IMPL_FACTORY_FOR(mat<T, 4, 4, is_col_major>)
template <typename Tp>
static constexpr auto ortho(Tp left, Tp right, Tp bottom, Tp top, Tp near,
                            Tp far) {
    auto result = Self::unit();
    const Tp rl = right - left;
    const Tp tb = top - bottom;
    const Tp fn = far - near;
    result.at(0, 0) = static_cast<Tp>(2) / rl;
    result.at(1, 1) = static_cast<Tp>(2) / tb;
    result.at(2, 2) = static_cast<Tp>(-2) / fn;
    result.at(0, 3) = -(right + left) / rl;
    result.at(1, 3) = -(top + bottom) / tb;
    result.at(2, 3) = -(far + near) / fn;
    return result;
}
IMPL_FACTORY_END();

IMPL_FACTORY_BEGIN(frustum_factory, typename T, bool is_col_major)
IMPL_FACTORY_FOR(mat<T, 4, 4, is_col_major>)
template <typename Tp>
static constexpr auto frustum(Tp left, Tp right, Tp bottom, Tp top, Tp near,
                              Tp far) {
    auto result = Self::zero();
    const Tp rl = right - left;
    const Tp tb = top - bottom;
    const Tp fn = far - near;
    result.at(0, 0) = (static_cast<Tp>(2) * near) / rl;
    result.at(1, 1) = (static_cast<Tp>(2) * near) / tb;
    result.at(0, 2) = (right + left) / rl;
    result.at(1, 2) = (top + bottom) / tb;
    result.at(2, 2) = -(far + near) / fn;
    result.at(2, 3) = -(static_cast<Tp>(2) * far * near) / fn;
    result.at(3, 2) = static_cast<Tp>(-1);
    return result;
}
IMPL_FACTORY_END();

}  // namespace xcmath
#include "xcmixin/scope_close.hpp"