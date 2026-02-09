#pragma once
#include <vcruntime_typeinfo.h>

#include <cstddef>
#include <type_traits>

#include "./alias.hpp"  // IWYU pragma: keep
#include "./functions.hpp"
#include "./number_meta.hpp"
#include "traits.hpp"

#define self (*static_cast<Self*>(this))
#define const_self (*static_cast<ConstSelf*>(this))
#define require_method(method)                     \
    static_assert(is_impl_method<Derived, method>, \
                  "Derived must be derived from " #method)

#define METHOD_INIT                     \
    using base_type = Base;             \
    using Self = std::decay_t<Derived>; \
    using ConstSelf = const Self;
#define METHOD_DEF_BEGIN(name)                 \
    template <typename Base, typename Derived> \
    struct name : Base {                       \
        METHOD_INIT
#define METHOD_DEF_END() \
    }                    \
    ;
#define METHOD_DECLARE(name) \
    METHOD_DEF_BEGIN(name)   \
    }
#ifdef __GNUC__
#define IMPL_METHOD_BEGIN(name, ext_template_params...) \
    template <typename Base, ext_template_params>       \
        struct name < Base,
#define IMPL_METHOD_BEGIN_WITH_REQUIRES(name, require_statement, \
                                        ext_template_params...)  \
    template <typename Base, ext_template_params>                \
        requires(require_statement)                              \
    struct name < Base,
#define IMPL_METHOD_FOR(cls...) \
    cls > : Base {              \
        using Self = cls;       \
        using ConstSelf = const std::remove_const_t<Self>;

#else
#define IMPL_METHOD_BEGIN(name, ...)      \
    template <typename Base, __VA_ARGS__> \
        struct name < Base,
#define IMPL_METHOD_BEGIN_WITH_REQUIRES(name, require_statement, ...) \
    template <typename Base, __VA_ARGS__>                             \
        requires(require_statement)                                   \
    struct name < Base,
#define IMPL_METHOD_FOR(...)      \
    __VA_ARGS__ > : Base {        \
        using Self = __VA_ARGS__; \
        using ConstSelf = const std::remove_const_t<Self>;
#endif
#define IMPL_METHOD_END() \
    }                     \
    ;
namespace xcmath {

struct EmptyBase {};

METHOD_DEF_BEGIN(size_method)
inline constexpr size_t size() const noexcept {
    return traits::length_properties<Derived>::length;
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

METHOD_DEF_BEGIN(clone_method)
inline constexpr auto clone() const noexcept { return const_self; }
METHOD_DEF_END()
METHOD_DEF_BEGIN(move_method)
inline constexpr decltype(auto) move() noexcept { return std::move(self); }
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
    require_method(clone_method);
    require_method(module_method);
    require_method(size_method);
    auto normalized = const_self.clone();
    auto module = normalized.module();
    for (size_t i = 0; i < const_self.size(); ++i) {
        normalized[i] /= module;
    }
    return normalized;
}
METHOD_DEF_END()

// Vector operations

METHOD_DEF_BEGIN(dot_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr auto dot(const T& other) const noexcept {
    require_method(size_method);
    auto result = number_meta::number_properties<
        std::decay_t<decltype(const_self[0] * other[0])>>::zero;
    for (size_t i = 0; i < const_self.size(); ++i) {
        result += const_self[i] * other[i];
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(distance_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr auto distance(const T& other) const noexcept {
    require_method(module_method);
    require_method(size_method);
    require_method(clone_method);
    auto diff = const_self.clone();
    for (size_t i = 0; i < const_self.size(); ++i) {
        diff[i] -= other[i];
    }
    return diff.module();
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(distance_squared_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr auto distance_squared(const T& other) const noexcept {
    auto result = number_meta::number_properties<
        std::decay_t<decltype((const_self[0] - other[0]) *
                              (const_self[0] - other[0]))>>::zero;
    for (size_t i = 0; i < const_self.size(); ++i) {
        auto diff = const_self[i] - other[i];
        result += diff * diff;
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(angle_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr auto angle(const T& other) const noexcept {
    require_method(dot_method);
    require_method(module_method);
    auto dot_product = const_self.dot(other);
    auto modules = const_self.module() * other.module();
    return xcmath::acos(dot_product / modules);
}
METHOD_DEF_END()

// Cross product for vec3 - base template (not usable)
METHOD_DEF_BEGIN(cross_product_method)
template <typename T>
    requires(traits::length_eq<Derived, T> &&
             traits::length_properties<Derived>::length == 3)
inline constexpr auto cross(const T& other) const noexcept {
    return decltype(const_self.clone()){
        const_self[1] * other[2] - const_self[2] * other[1],
        const_self[2] * other[0] - const_self[0] * other[2],
        const_self[0] * other[1] - const_self[1] * other[0]};
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(project_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr auto project(const T& onto) const noexcept {
    require_method(dot_method);
    auto onto_module_sq = number_meta::number_properties<
        std::decay_t<decltype(onto[0] * onto[0])>>::zero;
    for (size_t i = 0; i < onto.size(); ++i) {
        onto_module_sq += onto[i] * onto[i];
    }
    auto scalar = const_self.dot(onto) / onto_module_sq;
    auto result = const_self.clone();
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = onto[i] * scalar;
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(reflect_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr auto reflect(const T& normal) const noexcept {
    require_method(dot_method);
    auto dot_prod = const_self.dot(normal);
    auto result = const_self.clone();
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = result[i] -
                    number_meta::number_properties<decltype(dot_prod)>::unit *
                        2 * dot_prod * normal[i];
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(refract_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr auto refract(const T& normal, auto eta) const noexcept {
    require_method(dot_method);
    require_method(module_method);
    auto dot_prod = const_self.dot(normal);
    auto k = number_meta::number_properties<decltype(eta)>::unit -
             eta * eta *
                 (number_meta::number_properties<decltype(dot_prod)>::unit -
                  dot_prod * dot_prod);
    if (k < number_meta::number_properties<decltype(k)>::zero) {
        return Derived{number_meta::number_properties<
            std::decay_t<decltype(const_self[0])>>::zero};
    }
    auto result = const_self.clone();
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = eta * const_self[i] -
                    (eta * dot_prod + xcmath::sqrt(k)) * normal[i];
    }
    return result;
}
METHOD_DEF_END()

// Component-wise operations

METHOD_DEF_BEGIN(abs_method)
inline constexpr auto abs() const noexcept {
    auto result = const_self.clone();
    using value_type = std::decay_t<decltype(result[0])>;
    for (size_t i = 0; i < const_self.size(); ++i) {
        result[i] = result[i] < number_meta::number_properties<value_type>::zero
                        ? -result[i]
                        : result[i];
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(min_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr auto min(const T& other) const noexcept {
    auto result = const_self.clone();
    for (size_t i = 0; i < const_self.size(); ++i) {
        result[i] = result[i] < other[i] ? result[i] : other[i];
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(max_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr auto max(const T& other) const noexcept {
    auto result = const_self.clone();
    for (size_t i = 0; i < const_self.size(); ++i) {
        result[i] = result[i] > other[i] ? result[i] : other[i];
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(clamp_method)
template <typename T, typename U>
    requires(traits::length_eq<Derived, T> && traits::length_eq<Derived, U>)
inline constexpr auto clamp(const T& min_vec, const U& max_vec) const noexcept {
    auto result = const_self.clone();
    for (size_t i = 0; i < const_self.size(); ++i) {
        result[i] = result[i] < min_vec[i]   ? min_vec[i]
                    : result[i] > max_vec[i] ? max_vec[i]
                                             : result[i];
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(floor_method)
inline constexpr auto floor() const noexcept {
    auto result = const_self.clone();
    for (size_t i = 0; i < const_self.size(); ++i) {
        result[i] = xcmath::floor(result[i]);
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(ceil_method)
inline constexpr auto ceil() const noexcept {
    auto result = const_self.clone();
    for (size_t i = 0; i < const_self.size(); ++i) {
        result[i] = xcmath::ceil(result[i]);
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(round_method)
inline constexpr auto round() const noexcept {
    auto result = const_self.clone();
    for (size_t i = 0; i < const_self.size(); ++i) {
        result[i] = xcmath::round(result[i]);
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(fract_method)
inline constexpr auto fract() const noexcept {
    auto result = const_self.clone();
    for (size_t i = 0; i < const_self.size(); ++i) {
        result[i] = result[i] - xcmath::floor(result[i]);
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(sign_method)
inline constexpr auto sign() const noexcept {
    auto result = const_self.clone();
    using value_type = std::decay_t<decltype(result[0])>;
    for (size_t i = 0; i < const_self.size(); ++i) {
        result[i] =
            result[i] < number_meta::number_properties<value_type>::zero
                ? -number_meta::number_properties<value_type>::unit
                : (result[i] > number_meta::number_properties<value_type>::zero
                       ? number_meta::number_properties<value_type>::unit
                       : number_meta::number_properties<value_type>::zero);
    }
    return result;
}
METHOD_DEF_END()

// Comparison operations

METHOD_DEF_BEGIN(equal_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr bool equal(const T& other, auto epsilon) const noexcept {
    require_method(size_method);
    for (size_t i = 0; i < const_self.size(); ++i) {
        if (xcmath::fabs(const_self[i] - other[i]) > epsilon) {
            return false;
        }
    }
    return true;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(less_than_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr auto less_than(const T& other) const noexcept {
    auto result = const_self.clone();
    for (size_t i = 0; i < const_self.size(); ++i) {
        result[i] = const_self[i] < other[i];
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(greater_than_method)
template <typename T>
    requires(traits::length_eq<Derived, T>)
inline constexpr auto greater_than(const T& other) const noexcept {
    auto result = const_self.clone();
    for (size_t i = 0; i < const_self.size(); ++i) {
        result[i] = const_self[i] > other[i];
    }
    return result;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(any_method)
inline constexpr bool any() const noexcept {
    require_method(size_method);
    for (size_t i = 0; i < const_self.size(); ++i) {
        if (const_self[i]) {
            return true;
        }
    }
    return false;
}
METHOD_DEF_END()

METHOD_DEF_BEGIN(all_method)
inline constexpr bool all() const noexcept {
    require_method(size_method);
    for (size_t i = 0; i < const_self.size(); ++i) {
        if (!const_self[i]) {
            return false;
        }
    }
    return true;
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
#undef require_method