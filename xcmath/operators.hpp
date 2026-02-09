#pragma once
#include "methods.hpp"
#include "xcmath/traits.hpp"

#define self (*static_cast<Self*>(this))
#define const_self (*static_cast<ConstSelf*>(this))
#define require_method(method)                     \
    static_assert(is_impl_method<Derived, method>, \
                  "Derived must be derived from " #method)

namespace xcmath {

#define IMPL_DOUBLE_OP(name, op)                                \
    METHOD_DEF_BEGIN(name)                                      \
    template <typename T>                                       \
        requires(traits::length_eq<Derived, T>)                 \
    constexpr auto operator op(const T& other) const noexcept { \
        require_method(size_method);                            \
        require_method(impl_from_type_to_zero_method);          \
        auto result = Self::template impl_from_type_to_zero<    \
            decltype(const_self[0] op other[0])>();             \
        for (size_t i = 0; i < const_self.size(); ++i) {        \
            result[i] = (const_self[i] op other[i]);            \
        }                                                       \
        return result;                                          \
    }                                                           \
    METHOD_DEF_END()

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

using vec_double_operator_methods_recorder = method_recorder<
    operator_eq_method, operator_ne_method, operator_add_method,
    operator_sub_method, operator_div_method, operator_mul_method,
    operator_mod_method, operator_bit_and_method, operator_bit_or_method,
    operator_bit_xor_method, operator_shift_left_method,
    operator_shift_right_method, operator_and_method, operator_or_method>;

}  // namespace xcmath
#undef IMPL_DOUBLE_OP
#undef self
#undef const_self
#undef require_method