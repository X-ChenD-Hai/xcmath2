#pragma once
#include <algorithm>
#include <cmath>

#include "./number_meta.hpp"
#include "xcmixin/scope_open.hpp"

namespace xcmath {

//=============================================================================
// Forward Declarations
//=============================================================================

template <typename T, size_t N>
class vec;

template <typename T, size_t R, size_t C, bool ColMajor>
class mat;

//=============================================================================
// Helper Traits
//=============================================================================

namespace interpolation_details {

// Type trait to detect vec type
template <typename T>
struct is_vec : std::false_type {};

template <typename T, size_t N>
struct is_vec<vec<T, N>> : std::true_type {};

template <typename T>
constexpr bool is_vec_v = is_vec<T>::value;

// Type trait to detect mat type
template <typename T>
struct is_mat : std::false_type {};

template <typename T, size_t R, size_t C, bool ColMajor>
struct is_mat<mat<T, R, C, ColMajor>> : std::true_type {};

template <typename T>
constexpr bool is_mat_v = is_mat<T>::value;

// Get element type from vec or mat
template <typename T>
struct element_type {
    using type = T;
};

template <typename T, size_t N>
struct element_type<vec<T, N>> {
    using type = T;
};

template <typename T, size_t R, size_t C, bool ColMajor>
struct element_type<mat<T, R, C, ColMajor>> {
    using type = T;
};

template <typename T>
using element_type_t = typename element_type<T>::type;

}  // namespace interpolation_details

//=============================================================================
// Scalar Type Interpolation Functions
//=============================================================================

// Linear interpolation for scalar types
template <typename T>
inline constexpr T scalar_lerp(T a, T b,
                               interpolation_details::element_type_t<T> t) {
    return a + t * (b - a);
}

// Step function for scalar types
template <typename T>
inline constexpr T scalar_step(interpolation_details::element_type_t<T> edge,
                               T x) {
    return x < edge ? T(0) : T(1);
}

// Smoothstep for scalar types
template <typename T>
inline constexpr T scalar_smoothstep(
    interpolation_details::element_type_t<T> edge0,
    interpolation_details::element_type_t<T> edge1, T x) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type t = (static_cast<elem_type>(x) - edge0) / (edge1 - edge0);
    t = std::clamp(t, elem_type(0), elem_type(1));
    return static_cast<T>(t * t * (elem_type(3) - elem_type(2) * t));
}

// Smootherstep for scalar types (Perlin's improved smoothstep)
// Formula: 6t^5 - 15t^4 + 10t^3
template <typename T>
inline constexpr T scalar_smootherstep(
    interpolation_details::element_type_t<T> edge0,
    interpolation_details::element_type_t<T> edge1, T x) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type t = (static_cast<elem_type>(x) - edge0) / (edge1 - edge0);
    t = std::clamp(t, elem_type(0), elem_type(1));
    elem_type t2 = t * t;
    elem_type t3 = t2 * t;
    return static_cast<T>(elem_type(6) * t3 * t2 - elem_type(15) * t2 * t2 +
                          elem_type(10) * t3);
}

// Quadratic Bezier for scalar types
template <typename T>
inline constexpr T scalar_bezier(T p0, T p1, T p2,
                                 interpolation_details::element_type_t<T> t) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type one_minus_t = elem_type(1) - t;
    elem_type tt = t * t;
    elem_type one_minus_tt = one_minus_t * one_minus_t;
    return one_minus_tt * p0 + elem_type(2) * one_minus_t * t * p1 + tt * p2;
}

// Cubic Bezier for scalar types
template <typename T>
inline constexpr T scalar_bezier(T p0, T p1, T p2, T p3,
                                 interpolation_details::element_type_t<T> t) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type one_minus_t = elem_type(1) - t;
    elem_type tt = t * t;
    elem_type ttt = tt * t;
    elem_type one_minus_tt = one_minus_t * one_minus_t;
    elem_type one_minus_ttt = one_minus_tt * one_minus_t;
    return one_minus_ttt * p0 + elem_type(3) * one_minus_tt * t * p1 +
           elem_type(3) * one_minus_t * tt * p2 + ttt * p3;
}

// Catmull-Rom for scalar types
// Using the standard Catmull-Rom spline formulation
// h00 = 2t^3 - 3t^2 + 1
// h10 = t^3 - 2t^2 + t + tension * (t^3 - 2t^2 + t)
// h01 = -2t^3 + 3t^2
// h11 = t^3 - t^2 + tension * (t^3 - t^2)
// When tension = 0, this is the standard Catmull-Rom spline
template <typename T>
inline constexpr T scalar_catmull_rom(
    T p0, T p1, T p2, T p3, interpolation_details::element_type_t<T> t,
    interpolation_details::element_type_t<T> tension = 0.5) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type tt = t * t;
    elem_type ttt = tt * t;

    elem_type h00 = elem_type(2) * ttt - elem_type(3) * tt + elem_type(1);
    elem_type h10 = (ttt - elem_type(2) * tt + t) * (elem_type(1) - tension);
    elem_type h01 = elem_type(-2) * ttt + elem_type(3) * tt;
    elem_type h11 = (ttt - tt) * (elem_type(1) - tension);

    return h00 * p0 + h10 * p1 + h01 * p2 + h11 * p3;
}

// Hermite for scalar types
template <typename T>
inline constexpr T scalar_hermite(T p0, T m0, T p1, T m1,
                                  interpolation_details::element_type_t<T> t) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type tt = t * t;
    elem_type ttt = tt * t;

    elem_type h00 = elem_type(2) * ttt - elem_type(3) * tt + elem_type(1);
    elem_type h10 = ttt - elem_type(2) * tt + t;
    elem_type h01 = elem_type(-2) * ttt + elem_type(3) * tt;
    elem_type h11 = ttt - tt;

    return h00 * p0 + h10 * m0 + h01 * p1 + h11 * m1;
}

//=============================================================================
// Vector/Matrix Type Interpolation Functions
//=============================================================================

// Helper to create result type with zero initialization
template <typename T>
constexpr T create_zero_result() {
    if constexpr (interpolation_details::is_vec_v<T>) {
        using elem_type = interpolation_details::element_type_t<T>;
        T result{};
        for (size_t i = 0; i < T{}.size(); ++i) {
            result[i] = elem_type(0);
        }
        return result;
    } else {
        return T{};
    }
}

// Linear interpolation for vector/matrix types
template <typename T>
inline constexpr T vec_lerp(const T& a, const T& b,
                            interpolation_details::element_type_t<T> t) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type one_minus_t = elem_type(1) - t;
    T result = create_zero_result<T>();
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] * one_minus_t + b[i] * t;
    }
    return result;
}

// Step function for vector/matrix types
template <typename T>
inline constexpr T vec_step(interpolation_details::element_type_t<T> edge,
                            const T& x) {
    using elem_type = interpolation_details::element_type_t<T>;
    T result = create_zero_result<T>();
    for (size_t i = 0; i < x.size(); ++i) {
        result[i] = x[i] < edge ? elem_type(0) : elem_type(1);
    }
    return result;
}

// Smoothstep for vector/matrix types
template <typename T>
inline constexpr T vec_smoothstep(
    interpolation_details::element_type_t<T> edge0,
    interpolation_details::element_type_t<T> edge1, const T& x) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type denom = edge1 - edge0;
    T result = create_zero_result<T>();
    for (size_t i = 0; i < x.size(); ++i) {
        elem_type t = (x[i] - edge0) / denom;
        t = std::clamp(t, elem_type(0), elem_type(1));
        result[i] = t * t * (elem_type(3) - elem_type(2) * t);
    }
    return result;
}

// Smootherstep for vector/matrix types (Perlin's improved smoothstep)
// Formula: 6t^5 - 15t^4 + 10t^3
template <typename T>
inline constexpr T vec_smootherstep(
    interpolation_details::element_type_t<T> edge0,
    interpolation_details::element_type_t<T> edge1, const T& x) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type denom = edge1 - edge0;
    T result = create_zero_result<T>();
    for (size_t i = 0; i < x.size(); ++i) {
        elem_type t = (x[i] - edge0) / denom;
        t = std::clamp(t, elem_type(0), elem_type(1));
        elem_type t2 = t * t;
        elem_type t3 = t2 * t;
        result[i] = elem_type(6) * t3 * t2 - elem_type(15) * t2 * t2 +
                    elem_type(10) * t3;
    }
    return result;
}

// Quadratic Bezier for vector/matrix types
template <typename T>
inline constexpr T vec_bezier(const T& p0, const T& p1, const T& p2,
                              interpolation_details::element_type_t<T> t) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type one_minus_t = elem_type(1) - t;
    elem_type tt = t * t;
    elem_type one_minus_tt = one_minus_t * one_minus_t;

    T result = create_zero_result<T>();
    for (size_t i = 0; i < p0.size(); ++i) {
        result[i] = p0[i] * one_minus_tt +
                    p1[i] * (elem_type(2) * one_minus_t * t) + p2[i] * tt;
    }
    return result;
}

// Cubic Bezier for vector/matrix types
template <typename T>
inline constexpr T vec_bezier(const T& p0, const T& p1, const T& p2,
                              const T& p3,
                              interpolation_details::element_type_t<T> t) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type one_minus_t = elem_type(1) - t;
    elem_type tt = t * t;
    elem_type ttt = tt * t;
    elem_type one_minus_tt = one_minus_t * one_minus_t;
    elem_type one_minus_ttt = one_minus_tt * one_minus_t;

    T result = create_zero_result<T>();
    for (size_t i = 0; i < p0.size(); ++i) {
        result[i] = p0[i] * one_minus_ttt +
                    p1[i] * (elem_type(3) * one_minus_tt * t) +
                    p2[i] * (elem_type(3) * one_minus_t * tt) + p3[i] * ttt;
    }
    return result;
}

// Catmull-Rom for vector/matrix types
template <typename T>
inline constexpr T vec_catmull_rom(
    const T& p0, const T& p1, const T& p2, const T& p3,
    interpolation_details::element_type_t<T> t,
    interpolation_details::element_type_t<T> tension = 0.5) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type tt = t * t;
    elem_type ttt = tt * t;

    // Standard Catmull-Rom basis functions with tension
    elem_type h00 = elem_type(2) * ttt - elem_type(3) * tt + elem_type(1);
    elem_type h10 = (ttt - elem_type(2) * tt + t) * (elem_type(1) - tension);
    elem_type h01 = elem_type(-2) * ttt + elem_type(3) * tt;
    elem_type h11 = (ttt - tt) * (elem_type(1) - tension);

    T result = create_zero_result<T>();
    for (size_t i = 0; i < p0.size(); ++i) {
        result[i] = p0[i] * h00 + p1[i] * h10 + p2[i] * h01 + p3[i] * h11;
    }
    return result;
}

// Hermite for vector/matrix types
template <typename T>
inline constexpr T vec_hermite(const T& p0, const T& m0, const T& p1,
                               const T& m1,
                               interpolation_details::element_type_t<T> t) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type tt = t * t;
    elem_type ttt = tt * t;

    elem_type h00 = elem_type(2) * ttt - elem_type(3) * tt + elem_type(1);
    elem_type h10 = ttt - elem_type(2) * tt + t;
    elem_type h01 = elem_type(-2) * ttt + elem_type(3) * tt;
    elem_type h11 = ttt - tt;

    T result = create_zero_result<T>();
    for (size_t i = 0; i < p0.size(); ++i) {
        result[i] = p0[i] * h00 + m0[i] * h10 + p1[i] * h01 + m1[i] * h11;
    }
    return result;
}

//=============================================================================
// Public Interface Dispatchers
//=============================================================================

// Linear interpolation (lerp) - dispatches based on type
template <typename T>
inline constexpr T lerp(const T& a, const T& b,
                        interpolation_details::element_type_t<T> t) {
    if constexpr (interpolation_details::is_vec_v<T> ||
                  interpolation_details::is_mat_v<T>) {
        return vec_lerp(a, b, t);
    } else {
        return scalar_lerp(a, b, t);
    }
}

// Mix function (alias for lerp)
template <typename T>
inline constexpr T mix(const T& a, const T& b,
                       interpolation_details::element_type_t<T> t) {
    return lerp(a, b, t);
}

// Step function
template <typename T>
inline constexpr T step(interpolation_details::element_type_t<T> edge,
                        const T& x) {
    if constexpr (interpolation_details::is_vec_v<T> ||
                  interpolation_details::is_mat_v<T>) {
        return vec_step(edge, x);
    } else {
        return scalar_step(edge, x);
    }
}

// Smoothstep function
template <typename T>
inline constexpr T smoothstep(interpolation_details::element_type_t<T> edge0,
                              interpolation_details::element_type_t<T> edge1,
                              const T& x) {
    if constexpr (interpolation_details::is_vec_v<T> ||
                  interpolation_details::is_mat_v<T>) {
        return vec_smoothstep(edge0, edge1, x);
    } else {
        return scalar_smoothstep(edge0, edge1, x);
    }
}

// Smootherstep function (Perlin's improved smoothstep)
template <typename T>
inline constexpr T smootherstep(interpolation_details::element_type_t<T> edge0,
                                interpolation_details::element_type_t<T> edge1,
                                const T& x) {
    if constexpr (interpolation_details::is_vec_v<T> ||
                  interpolation_details::is_mat_v<T>) {
        return vec_smootherstep(edge0, edge1, x);
    } else {
        return scalar_smootherstep(edge0, edge1, x);
    }
}

// Quadratic Bezier curve
template <typename T>
inline constexpr T bezier(const T& p0, const T& p1, const T& p2,
                          interpolation_details::element_type_t<T> t) {
    if constexpr (interpolation_details::is_vec_v<T> ||
                  interpolation_details::is_mat_v<T>) {
        return vec_bezier(p0, p1, p2, t);
    } else {
        return scalar_bezier(p0, p1, p2, t);
    }
}

// Cubic Bezier curve
template <typename T>
inline constexpr T bezier(const T& p0, const T& p1, const T& p2, const T& p3,
                          interpolation_details::element_type_t<T> t) {
    if constexpr (interpolation_details::is_vec_v<T> ||
                  interpolation_details::is_mat_v<T>) {
        return vec_bezier(p0, p1, p2, p3, t);
    } else {
        return scalar_bezier(p0, p1, p2, p3, t);
    }
}

// Catmull-Rom spline interpolation
template <typename T>
inline constexpr T catmull_rom(
    const T& p0, const T& p1, const T& p2, const T& p3,
    interpolation_details::element_type_t<T> t,
    interpolation_details::element_type_t<T> tension = 0.0) {
    if constexpr (interpolation_details::is_vec_v<T> ||
                  interpolation_details::is_mat_v<T>) {
        return vec_catmull_rom(p0, p1, p2, p3, t, tension);
    } else {
        return scalar_catmull_rom(p0, p1, p2, p3, t, tension);
    }
}

// Hermite interpolation
template <typename T>
inline constexpr T hermite(const T& p0, const T& m0, const T& p1, const T& m1,
                           interpolation_details::element_type_t<T> t) {
    if constexpr (interpolation_details::is_vec_v<T> ||
                  interpolation_details::is_mat_v<T>) {
        return vec_hermite(p0, m0, p1, m1, t);
    } else {
        return scalar_hermite(p0, m0, p1, m1, t);
    }
}

//=============================================================================
// Spherical Interpolation Functions (for vec types only)
//=============================================================================

// Normalized linear interpolation (Nlerp)
template <typename T>
inline constexpr T nlerp(const T& q1, const T& q2,
                         interpolation_details::element_type_t<T> t) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type one_minus_t = elem_type(1) - t;

    T result = create_zero_result<T>();
    for (size_t i = 0; i < q1.size(); ++i) {
        result[i] = q1[i] * one_minus_t + q2[i] * t;
    }
    return result.normalize();
}

// Spherical linear interpolation (Slerp)
template <typename T>
inline constexpr T slerp(const T& q1, const T& q2,
                         interpolation_details::element_type_t<T> t) {
    using elem_type = interpolation_details::element_type_t<T>;
    elem_type dot = q1.dot(q2);

    elem_type sign = elem_type(1);
    if (dot < elem_type(0)) {
        dot = -dot;
        sign = elem_type(-1);
    }

    dot = std::clamp(dot, elem_type(-1), elem_type(1));

    elem_type half_theta = std::acos(dot);
    elem_type sin_theta = std::sin(half_theta);

    if (std::abs(sin_theta) < elem_type(1e-6)) {
        elem_type one_minus_t = elem_type(1) - t;
        T result = create_zero_result<T>();
        for (size_t i = 0; i < q1.size(); ++i) {
            result[i] = q1[i] * one_minus_t + q2[i] * sign * t;
        }
        return result;
    }

    elem_type ratio_a = std::sin((elem_type(1) - t) * half_theta) / sin_theta;
    elem_type ratio_b = std::sin(t * half_theta) * sign / sin_theta;

    T result = create_zero_result<T>();
    for (size_t i = 0; i < q1.size(); ++i) {
        result[i] = q1[i] * ratio_a + q2[i] * ratio_b;
    }
    return result;
}

}  // namespace xcmath

#include "xcmixin/scope_close.hpp"
