#pragma once
#include <type_traits>

#include "./methods.hpp"
#include "number_meta.hpp"

#define self (*static_cast<Derived*>(this))
#define const_self (*static_cast<const Derived*>(this))
#define require_method(method)                     \
    static_assert(is_impl_method<Derived, method>, \
                  "Derived must be derived from " #method)

namespace xcmath {

METHOD_DEF_BEGIN(trace_method)
constexpr auto trace() const noexcept {
    using result_type = std::decay_t<decltype(const_self[0, 0])>;
    result_type result = number_meta::number_properties<result_type>::zero;
    auto size = const_self.size() < const_self[0].size() ? const_self.size()
                                                         : const_self[0].size();
    for (size_t i = 0; i < size; ++i) {
        result += const_self[i, i];
    }
    return result;
}
METHOD_DEF_END()

}  // namespace xcmath

#undef self
#undef const_self
#undef require_method
