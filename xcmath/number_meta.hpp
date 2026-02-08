#pragma once

#include <cstdint>
#include <type_traits>
namespace xcmath ::number_meta {
template <typename T>
struct number_properties;
template <typename T>
struct number_properties<T&> : public number_properties<std::decay_t<T>> {};

template <typename number_type_, number_type_ zero_, number_type_ unit_>
struct number_properties_helper {
    using number_type = number_type_;
    static constexpr number_type zero = zero_;
    static constexpr number_type unit = unit_;
};
template <>
struct number_properties<uint8_t> : number_properties_helper<uint8_t, 0, 1> {};
template <>
struct number_properties<uint16_t> : number_properties_helper<uint16_t, 0, 1> {
};
template <>
struct number_properties<uint32_t> : number_properties_helper<uint32_t, 0, 1> {
};
template <>
struct number_properties<uint64_t> : number_properties_helper<uint64_t, 0, 1> {
};
template <>
struct number_properties<int8_t> : number_properties_helper<int8_t, 0, 1> {};
template <>
struct number_properties<int16_t> : number_properties_helper<int16_t, 0, 1> {};
template <>
struct number_properties<int32_t> : number_properties_helper<int32_t, 0, 1> {};
template <>
struct number_properties<int64_t> : number_properties_helper<int64_t, 0, 1> {};
template <>
struct number_properties<float> : number_properties_helper<float, 0.f, 1.f> {};
template <>
struct number_properties<double> : number_properties_helper<double, 0., 1.> {};

template <typename T>
struct constants_set {
    static constexpr long double PI = 3.14159265358979323846;
    static constexpr long double HALF_PI = PI / 2.0;
    static constexpr long double TAU = PI * 2.0;
    static constexpr long double QUARTER_PI = HALF_PI / 2.0;
    static constexpr long double DEGREE = 180.0 / PI;
    static constexpr long double RADIANS = 1.0 / DEGREE;
    static constexpr long double EPSILON = 1e-6;
    static constexpr size_t EPSILON_EXP = 6;
    static constexpr long double EPSILON_SQ = EPSILON * EPSILON;
};
}  // namespace xcmath::number_meta
