#include "xcmath/interpolation.hpp"

#include <gtest/gtest.h>

#include "xcmath/vec.hpp"

using namespace xcmath;

//=============================================================================
// Basic Interpolation Tests - Scalar
//=============================================================================

TEST(lerp_scalar_test, basic_values) {
    EXPECT_FLOAT_EQ(lerp(0.0f, 10.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(lerp(0.0f, 10.0f, 0.5f), 5.0f);
    EXPECT_FLOAT_EQ(lerp(0.0f, 10.0f, 1.0f), 10.0f);
    EXPECT_FLOAT_EQ(lerp(5.0f, 15.0f, 0.3f), 8.0f);
}

TEST(lerp_scalar_test, negative_values) {
    EXPECT_FLOAT_EQ(lerp(-10.0f, 10.0f, 0.5f), 0.0f);
    EXPECT_FLOAT_EQ(lerp(-5.0f, 5.0f, 0.0f), -5.0f);
    EXPECT_FLOAT_EQ(lerp(-5.0f, 5.0f, 1.0f), 5.0f);
}

TEST(mix_scalar_test, alias_of_lerp) {
    EXPECT_FLOAT_EQ(mix(0.0f, 10.0f, 0.5f), 5.0f);
    EXPECT_FLOAT_EQ(mix(2.0f, 8.0f, 0.25f), 3.5f);
}

TEST(step_scalar_test, basic_behavior) {
    EXPECT_FLOAT_EQ(step(0.5f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(step(0.5f, 0.5f), 1.0f);
    EXPECT_FLOAT_EQ(step(0.5f, 1.0f), 1.0f);
}

TEST(smoothstep_scalar_test, boundary_values) {
    EXPECT_FLOAT_EQ(smoothstep(0.0f, 1.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(smoothstep(0.0f, 1.0f, 1.0f), 1.0f);
    EXPECT_NEAR(smoothstep(0.0f, 1.0f, 0.5f), 0.5f, 1e-6f);
}

TEST(smootherstep_scalar_test, boundary_values) {
    EXPECT_FLOAT_EQ(smootherstep(0.0f, 1.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(smootherstep(0.0f, 1.0f, 1.0f), 1.0f);
    EXPECT_NEAR(smootherstep(0.0f, 1.0f, 0.5f), 0.5f, 1e-6f);
}

//=============================================================================
// Basic Interpolation Tests - Vector
//=============================================================================

TEST(lerp_vec_test, vec2_basic) {
    vec2<float> a{0.0f, 0.0f};
    vec2<float> b{10.0f, 20.0f};

    auto result_mid = lerp(a, b, 0.5f);
    EXPECT_FLOAT_EQ(result_mid[0], 5.0f);
    EXPECT_FLOAT_EQ(result_mid[1], 10.0f);

    auto result_start = lerp(a, b, 0.0f);
    EXPECT_FLOAT_EQ(result_start[0], 0.0f);
    EXPECT_FLOAT_EQ(result_start[1], 0.0f);

    auto result_end = lerp(a, b, 1.0f);
    EXPECT_FLOAT_EQ(result_end[0], 10.0f);
    EXPECT_FLOAT_EQ(result_end[1], 20.0f);
}

TEST(lerp_vec_test, vec3_basic) {
    vec3<float> a{1.0f, 2.0f, 3.0f};
    vec3<float> b{5.0f, 6.0f, 7.0f};

    auto result = lerp(a, b, 0.5f);
    EXPECT_FLOAT_EQ(result[0], 3.0f);
    EXPECT_FLOAT_EQ(result[1], 4.0f);
    EXPECT_FLOAT_EQ(result[2], 5.0f);
}

TEST(lerp_vec_test, vec4_basic) {
    vec4<float> a{0.0f, 0.0f, 0.0f, 0.0f};
    vec4<float> b{4.0f, 8.0f, 12.0f, 16.0f};

    auto result = lerp(a, b, 0.25f);
    EXPECT_FLOAT_EQ(result[0], 1.0f);
    EXPECT_FLOAT_EQ(result[1], 2.0f);
    EXPECT_FLOAT_EQ(result[2], 3.0f);
    EXPECT_FLOAT_EQ(result[3], 4.0f);
}

TEST(mix_vec_test, alias_of_lerp) {
    vec2<float> a{0.0f, 0.0f};
    vec2<float> b{10.0f, 20.0f};

    auto result = mix(a, b, 0.5f);
    EXPECT_FLOAT_EQ(result[0], 5.0f);
    EXPECT_FLOAT_EQ(result[1], 10.0f);
}

TEST(step_vec_test, vec2_basic) {
    vec2<float> x{0.5f, 1.5f};
    auto result = step(1.0f, x);

    EXPECT_FLOAT_EQ(result[0], 0.0f);
    EXPECT_FLOAT_EQ(result[1], 1.0f);
}

TEST(step_vec_test, vec3_basic) {
    vec3<float> x{0.0f, 1.0f, 2.0f};
    auto result = step(0.5f, x);

    EXPECT_FLOAT_EQ(result[0], 0.0f);
    EXPECT_FLOAT_EQ(result[1], 1.0f);
    EXPECT_FLOAT_EQ(result[2], 1.0f);
}

TEST(smoothstep_vec_test, vec2_basic) {
    vec2<float> x{0.0f, 1.0f};
    auto result = smoothstep(0.0f, 1.0f, x);

    EXPECT_FLOAT_EQ(result[0], 0.0f);
    EXPECT_FLOAT_EQ(result[1], 1.0f);
}

TEST(smoothstep_vec_test, vec2_midpoint) {
    vec2<float> x{0.5f, 0.5f};
    auto result = smoothstep(0.0f, 1.0f, x);

    EXPECT_NEAR(result[0], 0.5f, 1e-6f);
    EXPECT_NEAR(result[1], 0.5f, 1e-6f);
}

TEST(smootherstep_vec_test, vec2_basic) {
    vec2<float> x{0.0f, 1.0f};
    auto result = smootherstep(0.0f, 1.0f, x);

    EXPECT_FLOAT_EQ(result[0], 0.0f);
    EXPECT_FLOAT_EQ(result[1], 1.0f);
}

//=============================================================================
// Bezier Curve Tests
//=============================================================================

TEST(bezier_quadratic_test, scalar_basic) {
    // Quadratic Bezier: (1-t)^2 * p0 + 2*(1-t)*t * p1 + t^2 * p2
    // p0=0, p1=1, p2=2
    EXPECT_FLOAT_EQ(bezier(0.0f, 1.0f, 2.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(bezier(0.0f, 1.0f, 2.0f, 1.0f), 2.0f);
    EXPECT_NEAR(bezier(0.0f, 1.0f, 2.0f, 0.5f), 1.0f, 1e-6f);
}

TEST(bezier_quadratic_test, vec2_basic) {
    vec2<float> p0{0.0f, 0.0f};
    vec2<float> p1{1.0f, 2.0f};
    vec2<float> p2{2.0f, 0.0f};

    auto result_start = bezier(p0, p1, p2, 0.0f);
    EXPECT_FLOAT_EQ(result_start[0], 0.0f);
    EXPECT_FLOAT_EQ(result_start[1], 0.0f);

    auto result_end = bezier(p0, p1, p2, 1.0f);
    EXPECT_FLOAT_EQ(result_end[0], 2.0f);
    EXPECT_FLOAT_EQ(result_end[1], 0.0f);
}

TEST(bezier_cubic_test, scalar_basic) {
    // Cubic Bezier: (1-t)^3 * p0 + 3*(1-t)^2*t * p1 + 3*(1-t)*t^2 * p2 + t^3 *
    // p3
    EXPECT_FLOAT_EQ(bezier(0.0f, 0.0f, 1.0f, 1.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(bezier(0.0f, 0.0f, 1.0f, 1.0f, 1.0f), 1.0f);
    EXPECT_NEAR(bezier(0.0f, 0.0f, 1.0f, 1.0f, 0.5f), 0.5f, 1e-6f);
}

TEST(bezier_cubic_test, vec3_basic) {
    vec3<float> p0{0.0f, 0.0f, 0.0f};
    vec3<float> p1{1.0f, 2.0f, 1.0f};
    vec3<float> p2{2.0f, 1.0f, 1.0f};
    vec3<float> p3{3.0f, 0.0f, 0.0f};

    auto result_start = bezier(p0, p1, p2, p3, 0.0f);
    EXPECT_FLOAT_EQ(result_start[0], 0.0f);
    EXPECT_FLOAT_EQ(result_start[1], 0.0f);
    EXPECT_FLOAT_EQ(result_start[2], 0.0f);

    auto result_end = bezier(p0, p1, p2, p3, 1.0f);
    EXPECT_FLOAT_EQ(result_end[0], 3.0f);
    EXPECT_FLOAT_EQ(result_end[1], 0.0f);
    EXPECT_FLOAT_EQ(result_end[2], 0.0f);
}

//=============================================================================
// Catmull-Rom Spline Tests
//=============================================================================

TEST(catmull_rom_test, scalar_basic) {
    // At t=0 should return p1, at t=1 should return p2
    EXPECT_FLOAT_EQ(catmull_rom(0.0f, 0.0f, 1.0f, 1.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(catmull_rom(0.0f, 0.0f, 1.0f, 1.0f, 1.0f), 1.0f);
}

TEST(catmull_rom_test, scalar_midpoint) {
    // Standard Catmull-Rom (tension=0) at t=0.5 for (0,0,1,1):
    // h00=0.5, h10=0.125, h01=0.5, h11=-0.125
    // result = 0.5*0 + 0.125*0 + 0.5*1 + (-0.125)*1 = 0.375
    float result = catmull_rom(0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.0f);
    EXPECT_NEAR(result, 0.375f, 1e-6f);
}

TEST(catmull_rom_test, vec2_basic) {
    vec2<float> p0{0.0f, 0.0f};
    vec2<float> p1{0.0f, 0.0f};
    vec2<float> p2{1.0f, 1.0f};
    vec2<float> p3{1.0f, 1.0f};

    auto result_start = catmull_rom(p0, p1, p2, p3, 0.0f);
    EXPECT_FLOAT_EQ(result_start[0], 0.0f);
    EXPECT_FLOAT_EQ(result_start[1], 0.0f);

    auto result_end = catmull_rom(p0, p1, p2, p3, 1.0f);
    EXPECT_FLOAT_EQ(result_end[0], 1.0f);
    EXPECT_FLOAT_EQ(result_end[1], 1.0f);
}

TEST(catmull_rom_test, vec3_basic) {
    // Standard Catmull-Rom (tension=0) at t=0.5
    vec3<float> p0{0.0f, 0.0f, 0.0f};
    vec3<float> p1{1.0f, 1.0f, 1.0f};
    vec3<float> p2{2.0f, 2.0f, 2.0f};
    vec3<float> p3{3.0f, 3.0f, 3.0f};

    // result = p0*0.5 + p1*0.125 + p2*0.5 + p3*(-0.125)
    //        = 0*0.5 + 1*0.125 + 2*0.5 + 3*(-0.125)
    //        = 0 + 0.125 + 1 - 0.375 = 0.75
    auto result_mid = catmull_rom(p0, p1, p2, p3, 0.5f, 0.0f);
    EXPECT_NEAR(result_mid[0], 0.75f, 1e-5f);
    EXPECT_NEAR(result_mid[1], 0.75f, 1e-5f);
    EXPECT_NEAR(result_mid[2], 0.75f, 1e-5f);
}

TEST(catmull_rom_test, tension_parameter) {
    // Verify tension affects the result
    // tension=0: factor=1, result=0.375
    float tension_0 = catmull_rom(0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.0f);
    // tension=0.5: factor=0.5, h10=0.0625, h11=-0.0625, result=0.5 - 0.0625 =
    // 0.4375
    float tension_1 = catmull_rom(0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.5f);
    EXPECT_NE(tension_0, tension_1);
    EXPECT_NEAR(tension_0, 0.375f, 1e-6f);
    EXPECT_NEAR(tension_1, 0.4375f, 1e-6f);
}

//=============================================================================
// Hermite Interpolation Tests
//=============================================================================

TEST(hermite_test, scalar_basic) {
    // At t=0 should return p0, at t=1 should return p1
    EXPECT_FLOAT_EQ(hermite(0.0f, 1.0f, 1.0f, 1.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(hermite(0.0f, 1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
}

TEST(hermite_test, scalar_midpoint) {
    float result = hermite(0.0f, 1.0f, 1.0f, 1.0f, 0.5f);
    EXPECT_NEAR(result, 0.5f, 1e-6f);
}

TEST(hermite_test, vec2_basic) {
    vec2<float> p0{0.0f, 0.0f};
    vec2<float> m0{1.0f, 0.0f};
    vec2<float> p1{1.0f, 0.0f};
    vec2<float> m1{-1.0f, 0.0f};

    auto result_start = hermite(p0, m0, p1, m1, 0.0f);
    EXPECT_FLOAT_EQ(result_start[0], 0.0f);
    EXPECT_FLOAT_EQ(result_start[1], 0.0f);

    auto result_end = hermite(p0, m0, p1, m1, 1.0f);
    EXPECT_FLOAT_EQ(result_end[0], 1.0f);
    EXPECT_FLOAT_EQ(result_end[1], 0.0f);
}

TEST(hermite_test, vec3_basic) {
    vec3<float> p0{0.0f, 0.0f, 0.0f};
    vec3<float> m0{1.0f, 1.0f, 1.0f};
    vec3<float> p1{1.0f, 1.0f, 1.0f};
    vec3<float> m1{1.0f, 1.0f, 1.0f};

    auto result_mid = hermite(p0, m0, p1, m1, 0.5f);
    EXPECT_NEAR(result_mid[0], 0.5f, 1e-5f);
    EXPECT_NEAR(result_mid[1], 0.5f, 1e-5f);
    EXPECT_NEAR(result_mid[2], 0.5f, 1e-5f);
}

//=============================================================================
// Spherical Interpolation Tests
//=============================================================================

TEST(nlerp_test, vec3_unit_vectors) {
    vec3<float> q1{1.0f, 0.0f, 0.0f};
    vec3<float> q2{0.0f, 1.0f, 0.0f};

    auto result = nlerp(q1, q2, 0.5f);

    // Result should be normalized
    EXPECT_NEAR(result.module(), 1.0f, 1e-5f);

    // Result should be at 45 degrees (roughly)
    EXPECT_NEAR(result[0], result[1], 1e-5f);
}

TEST(nlerp_test, vec3_opposite_directions) {
    vec3<float> q1{1.0f, 0.0f, 0.0f};
    vec3<float> q2{0.0f, 1.0f, 0.0f};  // Perpendicular, not opposite

    auto result = nlerp(q1, q2, 0.5f);

    // Result should be normalized
    EXPECT_NEAR(result.module(), 1.0f, 1e-5f);

    // Result should be at 45 degrees
    EXPECT_NEAR(result[0], result[1], 1e-5f);
}

TEST(slerp_test, vec3_unit_vectors) {
    vec3<float> q1{1.0f, 0.0f, 0.0f};
    vec3<float> q2{0.0f, 1.0f, 0.0f};

    auto result = slerp(q1, q2, 0.5f);

    // Result should be normalized
    EXPECT_NEAR(result.module(), 1.0f, 1e-5f);

    // Result should be at 45 degrees (roughly)
    EXPECT_NEAR(result[0], result[1], 1e-5f);
}

TEST(slerp_test, vec3_parallel) {
    vec3<float> q1{1.0f, 0.0f, 0.0f};
    vec3<float> q2{1.0f, 0.0f, 0.0f};  // Same direction (unit vectors)

    auto result = slerp(q1, q2, 0.5f);

    // Result should be normalized (approximately)
    EXPECT_NEAR(result.module(), 1.0f, 1e-5f);
    EXPECT_NEAR(result[0], 1.0f, 1e-5f);
}

TEST(slerp_test, vec3_opposite) {
    // For truly opposite unit vectors, slerp behavior is undefined
    // (the rotation axis is undefined)
    // Use perpendicular vectors instead
    vec3<float> q1{1.0f, 0.0f, 0.0f};
    vec3<float> q2{0.0f, 1.0f, 0.0f};

    auto result_mid = slerp(q1, q2, 0.5f);
    auto result_start = slerp(q1, q2, 0.0f);
    auto result_end = slerp(q1, q2, 1.0f);

    // Result should be normalized
    EXPECT_NEAR(result_mid.module(), 1.0f, 1e-5f);
    EXPECT_NEAR(result_start[0], 1.0f, 1e-5f);
    EXPECT_NEAR(result_end[1], 1.0f, 1e-5f);
}

//=============================================================================
// Double Precision Tests
//=============================================================================

TEST(lerp_double_test, vec2_double_precision) {
    vec2<double> a{0.0, 0.0};
    vec2<double> b{1.0, 1.0};

    auto result = lerp(a, b, 0.5);
    EXPECT_NEAR(result[0], 0.5, 1e-10);
    EXPECT_NEAR(result[1], 0.5, 1e-10);
}

TEST(bezier_double_test, quadratic_double) {
    double result = bezier(0.0, 1.0, 2.0, 0.5);
    EXPECT_NEAR(result, 1.0, 1e-10);
}

//=============================================================================
// Edge Cases
//=============================================================================

TEST(lerp_edge_cases, same_point) {
    vec2<float> a{1.0f, 2.0f};
    vec2<float> b{1.0f, 2.0f};

    auto result = lerp(a, b, 0.5f);
    EXPECT_FLOAT_EQ(result[0], 1.0f);
    EXPECT_FLOAT_EQ(result[1], 2.0f);
}

TEST(step_edge_cases, at_edge) { EXPECT_FLOAT_EQ(step(0.5f, 0.5f), 1.0f); }

TEST(smoothstep_edge_cases, outside_range) {
    EXPECT_FLOAT_EQ(smoothstep(0.0f, 1.0f, -0.5f), 0.0f);
    EXPECT_FLOAT_EQ(smoothstep(0.0f, 1.0f, 1.5f), 1.0f);
}

TEST(smootherstep_edge_cases, outside_range) {
    EXPECT_FLOAT_EQ(smootherstep(0.0f, 1.0f, -0.5f), 0.0f);
    EXPECT_FLOAT_EQ(smootherstep(0.0f, 1.0f, 1.5f), 1.0f);
}
