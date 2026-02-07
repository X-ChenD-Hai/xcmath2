#include <gtest/gtest.h>

#include <cmath>

#include "xcmath/vec.hpp"

using namespace xcmath;

// Dot Product Tests

TEST(dot_product_test, dot_vec2) {
    vec2<float> v1{1.0f, 2.0f};
    vec2<float> v2{3.0f, 4.0f};
    EXPECT_FLOAT_EQ(v1.dot(v2), 1.0f * 3.0f + 2.0f * 4.0f);
}

TEST(dot_product_test, dot_vec3) {
    vec3<float> v1{1.0f, 2.0f, 3.0f};
    vec3<float> v2{4.0f, 5.0f, 6.0f};
    EXPECT_FLOAT_EQ(v1.dot(v2), 1.0f * 4.0f + 2.0f * 5.0f + 3.0f * 6.0f);
}

TEST(dot_product_test, dot_vec4) {
    vec4<float> v1{1.0f, 2.0f, 3.0f, 4.0f};
    vec4<float> v2{5.0f, 6.0f, 7.0f, 8.0f};
    EXPECT_FLOAT_EQ(v1.dot(v2),
                    1.0f * 5.0f + 2.0f * 6.0f + 3.0f * 7.0f + 4.0f * 8.0f);
}

TEST(dot_product_test, dot_perpendicular) {
    vec2<float> v1{1.0f, 0.0f};
    vec2<float> v2{0.0f, 1.0f};
    EXPECT_FLOAT_EQ(v1.dot(v2), 0.0f);
}

// Cross Product Tests

TEST(cross_product_test, cross_vec3) {
    vec3<float> v1{1.0f, 0.0f, 0.0f};
    vec3<float> v2{0.0f, 1.0f, 0.0f};
    auto result = v1.cross(v2);
    EXPECT_FLOAT_EQ(result[0], 0.0f);
    EXPECT_FLOAT_EQ(result[1], 0.0f);
    EXPECT_FLOAT_EQ(result[2], 1.0f);
}

TEST(cross_product_test, cross_anticommutative) {
    vec3<float> v1{1.0f, 2.0f, 3.0f};
    vec3<float> v2{4.0f, 5.0f, 6.0f};
    auto cross1 = v1.cross(v2);
    auto cross2 = v2.cross(v1);
    EXPECT_FLOAT_EQ(cross1[0], -cross2[0]);
    EXPECT_FLOAT_EQ(cross1[1], -cross2[1]);
    EXPECT_FLOAT_EQ(cross1[2], -cross2[2]);
}

// Distance Tests

TEST(distance_test, distance_vec2) {
    vec2<float> v1{0.0f, 0.0f};
    vec2<float> v2{3.0f, 4.0f};
    EXPECT_FLOAT_EQ(v1.distance(v2), 5.0f);
}

TEST(distance_test, distance_vec3) {
    vec3<float> v1{0.0f, 0.0f, 0.0f};
    vec3<float> v2{1.0f, 2.0f, 2.0f};
    EXPECT_FLOAT_EQ(v1.distance(v2), 3.0f);
}

TEST(distance_test, distance_zero) {
    vec3<float> v{1.0f, 2.0f, 3.0f};
    EXPECT_FLOAT_EQ(v.distance(v), 0.0f);
}

// Distance Squared Tests

TEST(distance_squared_test, distance_squared_vec2) {
    vec2<float> v1{0.0f, 0.0f};
    vec2<float> v2{3.0f, 4.0f};
    EXPECT_FLOAT_EQ(v1.distance_squared(v2), 25.0f);
}

TEST(distance_squared_test, distance_squared_vec3) {
    vec3<float> v1{0.0f, 0.0f, 0.0f};
    vec3<float> v2{1.0f, 2.0f, 2.0f};
    EXPECT_FLOAT_EQ(v1.distance_squared(v2), 9.0f);
}

// Angle Tests

TEST(angle_test, angle_parallel) {
    vec2<float> v1{1.0f, 0.0f};
    vec2<float> v2{2.0f, 0.0f};
    EXPECT_NEAR(v1.angle(v2), 0.0f, 1e-6f);
}

TEST(angle_test, angle_perpendicular) {
    vec2<float> v1{1.0f, 0.0f};
    vec2<float> v2{0.0f, 1.0f};
    EXPECT_NEAR(v1.angle(v2), std::acos(0.0f), 1e-6f);
}

// Project Tests

TEST(project_test, project_onto_x) {
    vec2<float> v{2.0f, 3.0f};
    vec2<float> onto{1.0f, 0.0f};
    auto result = v.project(onto);
    EXPECT_FLOAT_EQ(result[0], 2.0f);
    EXPECT_FLOAT_EQ(result[1], 0.0f);
}

TEST(project_test, project_onto_diagonal) {
    vec2<float> v{2.0f, 0.0f};
    vec2<float> onto{1.0f, 1.0f};
    auto result = v.project(onto);
    EXPECT_NEAR(result[0], 1.0f, 1e-6f);
    EXPECT_NEAR(result[1], 1.0f, 1e-6f);
}

// Reflect Tests

TEST(reflect_test, reflect_x_axis) {
    vec2<float> v{1.0f, 1.0f};
    vec2<float> normal{0.0f, 1.0f};
    auto result = v.reflect(normal);
    EXPECT_NEAR(result[0], 1.0f, 1e-6f);
    EXPECT_NEAR(result[1], -1.0f, 1e-6f);
}

// Component-wise Tests

TEST(component_wise_test, abs_positive) {
    vec2<float> v{1.0f, 2.0f};
    auto result = v.abs();
    EXPECT_FLOAT_EQ(result[0], 1.0f);
    EXPECT_FLOAT_EQ(result[1], 2.0f);
}

TEST(component_wise_test, abs_negative) {
    vec2<float> v{-1.0f, -2.0f};
    auto result = v.abs();
    EXPECT_FLOAT_EQ(result[0], 1.0f);
    EXPECT_FLOAT_EQ(result[1], 2.0f);
}

TEST(component_wise_test, min) {
    vec2<float> v1{1.0f, 5.0f};
    vec2<float> v2{3.0f, 2.0f};
    auto result = v1.min(v2);
    EXPECT_FLOAT_EQ(result[0], 1.0f);
    EXPECT_FLOAT_EQ(result[1], 2.0f);
}

TEST(component_wise_test, max) {
    vec2<float> v1{1.0f, 5.0f};
    vec2<float> v2{3.0f, 2.0f};
    auto result = v1.max(v2);
    EXPECT_FLOAT_EQ(result[0], 3.0f);
    EXPECT_FLOAT_EQ(result[1], 5.0f);
}

TEST(component_wise_test, clamp) {
    vec2<float> v{-1.0f, 5.0f};
    vec2<float> min_vec{0.0f, 1.0f};
    vec2<float> max_vec{2.0f, 4.0f};
    auto result = v.clamp(min_vec, max_vec);
    EXPECT_FLOAT_EQ(result[0], 0.0f);
    EXPECT_FLOAT_EQ(result[1], 4.0f);
}

TEST(component_wise_test, floor) {
    vec2<float> v{1.5f, 2.8f};
    auto result = v.floor();
    EXPECT_FLOAT_EQ(result[0], 1.0f);
    EXPECT_FLOAT_EQ(result[1], 2.0f);
}

TEST(component_wise_test, ceil) {
    vec2<float> v{1.2f, 2.8f};
    auto result = v.ceil();
    EXPECT_FLOAT_EQ(result[0], 2.0f);
    EXPECT_FLOAT_EQ(result[1], 3.0f);
}

TEST(component_wise_test, round) {
    vec2<float> v{1.4f, 2.6f};
    auto result = v.round();
    EXPECT_FLOAT_EQ(result[0], 1.0f);
    EXPECT_FLOAT_EQ(result[1], 3.0f);
}

TEST(component_wise_test, fract) {
    vec2<float> v{1.25f, 2.75f};
    auto result = v.fract();
    EXPECT_NEAR(result[0], 0.25f, 1e-6f);
    EXPECT_NEAR(result[1], 0.75f, 1e-6f);
}

TEST(component_wise_test, sign) {
    vec3<float> v{-5.0f, 0.0f, 3.0f};
    auto result = v.sign();
    EXPECT_FLOAT_EQ(result[0], -1.0f);
    EXPECT_FLOAT_EQ(result[1], 0.0f);
    EXPECT_FLOAT_EQ(result[2], 1.0f);
}

// Comparison Tests

TEST(comparison_test, equal_exact) {
    vec2<float> v1{1.0f, 2.0f};
    vec2<float> v2{1.0f, 2.0f};
    EXPECT_TRUE(v1.equal(v2, 0.0f));
}

TEST(comparison_test, equal_approximate) {
    vec2<float> v1{1.0f, 2.0f};
    vec2<float> v2{1.0f + 1e-7f, 2.0f - 1e-7f};
    EXPECT_TRUE(v1.equal(v2, 1e-5f));
}

TEST(comparison_test, not_equal) {
    vec2<float> v1{1.0f, 2.0f};
    vec2<float> v2{1.0f, 3.0f};
    EXPECT_FALSE(v1.equal(v2, 0.0f));
}

TEST(comparison_test, less_than) {
    vec2<float> v1{1.0f, 2.0f};
    vec2<float> v2{3.0f, 4.0f};
    auto result = v1.less_than(v2);
    EXPECT_FLOAT_EQ(result[0], 1.0f);
    EXPECT_FLOAT_EQ(result[1], 1.0f);
}

TEST(comparison_test, greater_than) {
    vec2<float> v1{3.0f, 4.0f};
    vec2<float> v2{1.0f, 2.0f};
    auto result = v1.greater_than(v2);
    EXPECT_FLOAT_EQ(result[0], 1.0f);
    EXPECT_FLOAT_EQ(result[1], 1.0f);
}

TEST(comparison_test, any) {
    vec2<float> v{0.0f, 1.0f};
    EXPECT_TRUE(v.any());
}

TEST(comparison_test, any_all_false) {
    vec2<float> v{0.0f, 0.0f};
    EXPECT_FALSE(v.any());
}

TEST(comparison_test, all) {
    vec2<float> v{1.0f, 1.0f};
    EXPECT_TRUE(v.all());
}

TEST(comparison_test, all_not_all) {
    vec2<float> v{1.0f, 0.0f};
    EXPECT_FALSE(v.all());
}
