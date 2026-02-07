#pragma once
#include <vcruntime_typeinfo.h>

#include <cstddef>
#include <type_traits>

#include "./functions.hpp"
#include "./number_meta.hpp"

#define self (*static_cast<Derived*>(this))
#define const_self (*static_cast<const Derived*>(this))
#define require_method(method)                     \
    static_assert(is_impl_method<Derived, method>, \
                  "Derived must be derived from " #method)

#define METHOD_INIT using base_type = Base;
#define METHOD_DEF_BEGIN(name)                 \
    template <typename Base, typename Derived> \
    struct name : Base {                       \
        METHOD_INIT
#define METHOD_DEF_END() \
    }                    \
    ;
namespace xcmath {

template <template <typename, typename> class... methods>
struct method_recorder {};

template <template <typename, typename> class method, typename recorder>
static constexpr bool has_method = false;

template <template <typename, typename> class method,
          template <typename, typename> class... methods>
static constexpr bool has_method<method, method_recorder<methods...>> =
    (std::is_same_v<method<void, void>, methods<void, void>> || ...);

template <typename Derived, template <typename, typename> class method>
static constexpr bool is_impl_method =
    has_method<method, typename Derived::method_recorder>;

struct EmptyBase {};

METHOD_DEF_BEGIN(size_method)
inline constexpr size_t size() const noexcept {
    static_assert(false, "size_method not implemented");
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(zero_factory_method)
METHOD_INIT static inline constexpr auto zero() noexcept {
    return number_meta::number_properties<Derived>::zero;
}
METHOD_DEF_END()
METHOD_DEF_BEGIN(unit_factory_method)
static inline constexpr auto unit() noexcept {
    return number_meta::number_properties<Derived>::unit;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(module_method)
inline constexpr auto module() const noexcept {
    require_method(size_method);
    using item_type = std::decay_t<decltype(const_self[0])>;
    item_type module = number_meta::number_properties<item_type>::zero;
    for (size_t i = 0; i < const_self.size(); ++i) {
        module += const_self[i] * const_self[i];
    }
    return xcmath::sqrt(module);
}
METHOD_DEF_END()
METHOD_DEF_BEGIN(normalize_method)
inline constexpr auto normalize() const noexcept {
    auto normalized = const_self;
    auto module = normalized.module();
    for (size_t i = 0; i < const_self.size(); ++i) {
        normalized[i] /= module;
    }
    return normalized;
}
METHOD_DEF_END()

template <typename Derived, template <typename, typename> class... methods>
struct impl_methods_helper;
template <typename Derived, template <typename, typename> class... methods>
using impl_methods = typename impl_methods_helper<Derived, methods...>::type;
template <typename Derived, template <typename, typename> class method>
struct impl_methods_helper<Derived, method> {
    struct type : method<EmptyBase, Derived> {
        using method_recorder = method_recorder<method>;
    };
};
template <typename Derived, template <typename, typename> class method,
          template <typename, typename> class... methods>
struct impl_methods_helper<Derived, method, methods...> {
    struct type : method<impl_methods<Derived, methods...>, Derived> {
        using method_recorder = method_recorder<method, methods...>;
    };
};
}  // namespace xcmath

#undef self
#undef const_self