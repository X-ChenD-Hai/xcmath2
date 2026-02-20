#pragma once
#include "./number_meta.hpp"
#include <xcmixin/xcmixin.hpp>

namespace xcmath {
XCMIXIN_DEF_BEGIN(impl_from_type_to_zero_factory)
template <typename T>
static constexpr auto impl_from_type_to_zero() noexcept {
    static_assert(false, "impl_from_type_to_zero_factory must be implemented");
}
template <typename T, size_t>
static constexpr auto impl_from_type_to_zero() noexcept {
    static_assert(false, "impl_from_type_to_zero_factory must be implemented");
}
XCMIXIN_DEF_END()

XCMIXIN_DEF_BEGIN(zero_factory)
static inline constexpr auto zero() noexcept {
    return number_meta::number_properties<Derived>::zero;
}
XCMIXIN_DEF_END()
XCMIXIN_DEF_BEGIN(unit_factory)
static inline constexpr auto unit() noexcept {
    return number_meta::number_properties<Derived>::unit;
}
XCMIXIN_DEF_END()
using vec_factory_methods_recorder =
    xcmixin::mixin_recorder<unit_factory, zero_factory,
                             impl_from_type_to_zero_factory>;

}  // namespace xcmath
