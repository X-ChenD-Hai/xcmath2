#pragma once
#include "./number_meta.hpp"
#include "./traits.hpp"
#include "./utility/compile_utils.hpp"

namespace xcmath {
FACTORY_DEF_BEGIN(impl_from_type_to_zero_factory)
template <typename T>
static constexpr auto impl_from_type_to_zero() noexcept {
    static_assert(false, "impl_from_type_to_zero_factory must be implemented");
}
template <typename T, size_t>
static constexpr auto impl_from_type_to_zero() noexcept {
    static_assert(false, "impl_from_type_to_zero_factory must be implemented");
}
FACTORY_DEF_END()

FACTORY_DEF_BEGIN(zero_factory)
static inline constexpr auto zero() noexcept {
    return number_meta::number_properties<Derived>::zero;
}
FACTORY_DEF_END()
FACTORY_DEF_BEGIN(unit_factory)
static inline constexpr auto unit() noexcept {
    return number_meta::number_properties<Derived>::unit;
}
FACTORY_DEF_END()
using vec_factory_methods_recorder =
    method_recorder<unit_factory, zero_factory, impl_from_type_to_zero_factory>;

}  // namespace xcmath