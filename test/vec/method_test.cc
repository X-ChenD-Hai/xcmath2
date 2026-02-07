#include <gtest/gtest.h>

#include <cmath>

#include "xcmath/vec.hpp"

using namespace xcmath;

// Size Method Tests

TEST(vec_method_test, size_vec2) {
    vec2<float> v;
    EXPECT_EQ(v.size(), 2u);
}

TEST(vec_method_test, size_vec3) {
    vec3<float> v;
    EXPECT_EQ(v.size(), 3u);
}

TEST(vec_method_test, size_vec4) {
    vec4<float> v;
    EXPECT_EQ(v.size(), 4u);
}

TEST(vec_method_test, size_custom) {
    vec<int, 10> v;
    EXPECT_EQ(v.size(), 10u);
}

// Zero Factory Method Tests

TEST(vec_method_test, zero_factory_vec2) {
    auto v = vec2<float>::zero();
    EXPECT_FLOAT_EQ(v[0], 0.0f);
    EXPECT_FLOAT_EQ(v[1], 0.0f);
}

TEST(vec_method_test, zero_factory_vec3) {
    auto v = vec3<float>::zero();
    EXPECT_FLOAT_EQ(v[0], 0.0f);
    EXPECT_FLOAT_EQ(v[1], 0.0f);
    EXPECT_FLOAT_EQ(v[2], 0.0f);
}

TEST(vec_method_test, zero_factory_vec4) {
    auto v = vec4<double>::zero();
    EXPECT_DOUBLE_EQ(v[0], 0.0);
    EXPECT_DOUBLE_EQ(v[1], 0.0);
    EXPECT_DOUBLE_EQ(v[2], 0.0);
    EXPECT_DOUBLE_EQ(v[3], 0.0);
}

// Module Method Tests

TEST(vec_method_test, module_vec2) {
    vec2<float> v{3.0f, 4.0f};
    EXPECT_FLOAT_EQ(v.module(), 5.0f);
}

TEST(vec_method_test, module_vec3) {
    vec3<float> v{1.0f, 2.0f, 2.0f};
    EXPECT_FLOAT_EQ(v.module(), 3.0f);
}

TEST(vec_method_test, module_vec4) {
    vec4<double> v{1.0, 2.0, 3.0, 4.0};
    double expected = std::sqrt(1.0 + 4.0 + 9.0 + 16.0);
    EXPECT_DOUBLE_EQ(v.module(), expected);
}

TEST(vec_method_test, module_unit_vector) {
    vec3<float> v{1.0f, 0.0f, 0.0f};
    EXPECT_FLOAT_EQ(v.module(), 1.0f);
}

TEST(vec_method_test, module_zero_vector) {
    vec3<float> v{0.0f, 0.0f, 0.0f};
    EXPECT_FLOAT_EQ(v.module(), 0.0f);
}

// Normalize Method Tests

TEST(vec_method_test, normalize_vec2) {
    vec2<float> v{3.0f, 4.0f};
    auto n = v.normalize();
    EXPECT_FLOAT_EQ(n[0], 0.6f);
    EXPECT_FLOAT_EQ(n[1], 0.8f);
}

TEST(vec_method_test, normalize_vec3) {
    vec3<float> v{1.0f, 2.0f, 2.0f};
    auto n = v.normalize();
    EXPECT_FLOAT_EQ(n[0], 1.0f / 3.0f);
    EXPECT_FLOAT_EQ(n[1], 2.0f / 3.0f);
    EXPECT_FLOAT_EQ(n[2], 2.0f / 3.0f);
}

TEST(vec_method_test, normalize_unit_length) {
    vec3<double> v{1.0, 2.0, 3.0};
    auto n = v.normalize();
    EXPECT_NEAR(n.module(), 1.0, 1e-10);
}

TEST(vec_method_test, normalize_preserves_direction) {
    vec4<float> v{2.0f, 4.0f, 6.0f, 8.0f};
    auto n = v.normalize();
    float ratio = v[0] / n[0];
    EXPECT_NEAR(v[1] / n[1], ratio, 1e-5f);
    EXPECT_NEAR(v[2] / n[2], ratio, 1e-5f);
    EXPECT_NEAR(v[3] / n[3], ratio, 1e-5f);
}
