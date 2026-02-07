#pragma once
#include <cstddef>
#include <type_traits>

#include "./functions.hpp"
#include "./number_meta.hpp"

namespace xcmath {

template <typename T>
constexpr size_t methods_size{0};

struct EmptyBase {};

template <typename Base, typename Derived>
struct zero_factory_method : Base {
    static inline constexpr auto zero() noexcept {
        return number_meta::number_properties<Derived>::zero;
    }
};
template <typename Base, typename Derived>
struct unit_factory_method : Base {
    static inline constexpr auto unit() noexcept {
        return number_meta::number_properties<Derived>::unit;
    }
};

template <typename Base, typename Derived>
struct module_method : Base {
    inline constexpr auto module() const noexcept {
        using item_type =
            std::decay_t<decltype(static_cast<const Derived*>(this)->operator[](
                0))>;
        item_type module = number_meta::number_properties<item_type>::zero;
        for (size_t i = 0; i < methods_size<Derived>; ++i) {
            module += static_cast<const Derived*>(this)->operator[](i) *
                      static_cast<const Derived*>(this)->operator[](i);
        }
        return xcmath::sqrt(module);
    }
};

template <typename Base, typename Derived>
struct normalize_method {
    inline constexpr auto normalize() const noexcept {
        auto normalized = *static_cast<const Derived*>(this);
        auto module = normalized.module();
        for (size_t i = 0; i < methods_size<Derived>; ++i) {
            normalized[i] /= module;
        }
        return normalized;
    }
};

template <typename Derived, template <typename, typename> class... methods>
struct impl_methods_helper;
template <typename Derived, template <typename, typename> class... methods>
using impl_methods = typename impl_methods_helper<Derived, methods...>::type;
template <typename Derived, template <typename, typename> class method>
struct impl_methods_helper<Derived, method> {
    using type = method<EmptyBase, Derived>;
};
template <typename Derived, template <typename, typename> class method,
          template <typename, typename> class... methods>
struct impl_methods_helper<Derived, method, methods...> {
    struct type : method<impl_methods<Derived, methods...>, Derived> {};
};
}  // namespace xcmath