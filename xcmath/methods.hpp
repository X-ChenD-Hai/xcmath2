#pragma once
#include <vcruntime_typeinfo.h>

#include <cstddef>
#include <type_traits>

#include "./alias.hpp"  // IWYU pragma: keep
#include "./functions.hpp"
#include "./number_meta.hpp"
#include "traits.hpp"
#include "xcmixin/scope_open.hpp"

namespace xcmath {

struct EmptyBase {};
METHOD_DEF_BEGIN(size_method)
inline constexpr size_t size() const noexcept {
    return traits::length_properties<Derived>::length;
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
    xcmixin_require_method(size_method);
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
    xcmixin_require_method(clone_method);
    xcmixin_require_method(module_method);
    xcmixin_require_method(size_method);
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
    xcmixin_require_method(size_method);
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
    xcmixin_require_method(module_method);
    xcmixin_require_method(size_method);
    xcmixin_require_method(clone_method);
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
    xcmixin_require_method(dot_method);
    xcmixin_require_method(module_method);
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
    xcmixin_require_method(dot_method);
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
    xcmixin_require_method(dot_method);
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
    xcmixin_require_method(dot_method);
    xcmixin_require_method(module_method);
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
template <typename T, typename U>
    requires(traits::length_eq<Derived, T>)
inline constexpr bool equal(
    const T& other,
    const U& epsilon = number_meta::constants_set<U>::epsilon) const noexcept {
    xcmixin_require_method(size_method);
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
    xcmixin_require_method(size_method);
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
    xcmixin_require_method(size_method);
    for (size_t i = 0; i < const_self.size(); ++i) {
        if (!const_self[i]) {
            return false;
        }
    }
    return true;
}
METHOD_DEF_END()

using vec_member_methods_recorder = xcmixin::method_recorder<
    size_method, clone_method, move_method, module_method, normalize_method,
    dot_method, distance_method, distance_squared_method, angle_method,
    cross_product_method, project_method, reflect_method, refract_method,
    abs_method, min_method, max_method, clamp_method, floor_method, ceil_method,
    round_method, fract_method, sign_method, equal_method, less_than_method,
    greater_than_method, any_method, all_method>;
}  // namespace xcmath

#include "xcmixin/scope_close.hpp"