#pragma once
#include "./factories.hpp"
#include "./methods.hpp"
#include "./traits.hpp"
#include <xcmixin/xcmixin.hpp>

namespace xcmath {

#define IMPL_DOUBLE_OP(name, op)                                 \
    XCMIXIN_DEF_BEGIN(name)                                      \
    template <typename T>                                        \
        requires(traits::length_eq<Derived, T>)                  \
    constexpr auto operator op(const T& other) const noexcept {  \
        xcmixin_require_mixin(size_method);                     \
        xcmixin_require_mixin(impl_from_type_to_zero_factory);  \
        auto result = Self::template impl_from_type_to_zero<     \
            decltype(xcmixin_const_self[0] op other[0])>();      \
        for (size_t i = 0; i < xcmixin_const_self.size(); ++i) { \
            result[i] = (xcmixin_const_self[i] op other[i]);     \
        }                                                        \
        return result;                                           \
    }                                                            \
    XCMIXIN_DEF_END()

IMPL_DOUBLE_OP(operator_add_method, +);
IMPL_DOUBLE_OP(operator_sub_method, -);
IMPL_DOUBLE_OP(operator_div_method, /);
IMPL_DOUBLE_OP(operator_mul_method, *);
IMPL_DOUBLE_OP(operator_mod_method, %);
IMPL_DOUBLE_OP(operator_bit_and_method, &);
IMPL_DOUBLE_OP(operator_bit_or_method, |);
IMPL_DOUBLE_OP(operator_bit_xor_method, ^);
IMPL_DOUBLE_OP(operator_shift_left_method, <<);
IMPL_DOUBLE_OP(operator_shift_right_method, >>);
IMPL_DOUBLE_OP(operator_and_method, &&);
IMPL_DOUBLE_OP(operator_or_method, ||);
IMPL_DOUBLE_OP(operator_eq_method, ==)
IMPL_DOUBLE_OP(operator_ne_method, !=)
IMPL_DOUBLE_OP(operator_gt_method, >)
IMPL_DOUBLE_OP(operator_ge_method, >=)
IMPL_DOUBLE_OP(operator_lt_method, <)
IMPL_DOUBLE_OP(operator_le_method, <=)

XCMIXIN_DEF_BEGIN(operator_bitnot_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
constexpr auto operator~() const noexcept {
    xcmixin_require_mixin(size_method);
    xcmixin_require_mixin(impl_from_type_to_zero_factory);
    auto result = Self::template impl_from_type_to_zero<
        decltype(~xcmixin_const_self[0])>();
    for (size_t i = 0; i < xcmixin_const_self.size(); ++i) {
        result[i] = ~xcmixin_const_self[i];
    }
    return result;
}
XCMIXIN_DEF_END()

XCMIXIN_DEF_BEGIN(operator_not_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
constexpr auto operator!() const noexcept {
    xcmixin_require_mixin(size_method);
    xcmixin_require_mixin(impl_from_type_to_zero_factory);
    auto result = Self::template impl_from_type_to_zero<
        decltype(!xcmixin_const_self[0])>();
    for (size_t i = 0; i < xcmixin_const_self.size(); ++i) {
        result[i] = !xcmixin_const_self[i];
    }
    return result;
}
XCMIXIN_DEF_END()

XCMIXIN_DEF_BEGIN(operator_inc_method)
constexpr auto operator++() noexcept {
    xcmixin_require_mixin(size_method);
    xcmixin_require_mixin(clone_method);
    auto result = xcmixin_self.clone();
    for (size_t i = 0; i < xcmixin_const_self.size(); ++i) {
        result[i]++;
    }
    return result;
}
XCMIXIN_DEF_END()

XCMIXIN_DEF_BEGIN(operator_dec_method)
constexpr auto operator--() noexcept {
    xcmixin_require_mixin(size_method);
    xcmixin_require_mixin(clone_method);
    auto result = xcmixin_self.clone();
    for (size_t i = 0; i < xcmixin_const_self.size(); ++i) {
        result[i]--;
    }
    return result;
}
XCMIXIN_DEF_END()

XCMIXIN_DEF_BEGIN(operator_pre_inc_method)
constexpr auto operator++(int) noexcept {
    xcmixin_require_mixin(size_method);
    for (size_t i = 0; i < xcmixin_const_self.size(); ++i) {
        ++xcmixin_self[i];
    }
    return xcmixin_self;
}
XCMIXIN_DEF_END()
XCMIXIN_DEF_BEGIN(operator_pre_dec_method)
constexpr auto operator--(int) noexcept {
    xcmixin_require_mixin(size_method);
    for (size_t i = 0; i < xcmixin_const_self.size(); ++i) {
        --xcmixin_self[i];
    }
    return xcmixin_self;
}
XCMIXIN_DEF_END()

using vec_single_operator_methods_recorder =
    xcmixin::mixin_recorder<operator_bitnot_method, operator_not_method,
                             operator_inc_method, operator_dec_method,
                             operator_pre_inc_method, operator_pre_dec_method>;

using vec_double_operator_methods_recorder = xcmixin::mixin_recorder<
    operator_eq_method, operator_ne_method, operator_add_method,
    operator_sub_method, operator_div_method, operator_mul_method,
    operator_mod_method, operator_bit_and_method, operator_bit_or_method,
    operator_bit_xor_method, operator_shift_left_method,
    operator_shift_right_method, operator_and_method, operator_or_method>;

}  // namespace xcmath
#undef IMPL_DOUBLE_OP
