#include <gtest/gtest.h>

#include "xcmath/vec.hpp"

using namespace xcmath;

TEST(vec_constructor_test, default_constructor) {
    vec3<float> v;
    EXPECT_FLOAT_EQ(v[0], 0.0f);
    EXPECT_FLOAT_EQ(v[1], 0.0f);
    EXPECT_FLOAT_EQ(v[2], 0.0f);
}

TEST(vec_constructor_test, default_constructor_int) {
    vec3<int> v;
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 0);
    EXPECT_EQ(v[2], 0);
}

TEST(vec_constructor_test, fill_constructor) {
    vec3<float> v(5.0f);
    EXPECT_FLOAT_EQ(v[0], 5.0f);
    EXPECT_FLOAT_EQ(v[1], 5.0f);
    EXPECT_FLOAT_EQ(v[2], 5.0f);
}

TEST(vec_constructor_test, fill_constructor_int) {
    vec4<int> v(42);
    EXPECT_EQ(v[0], 42);
    EXPECT_EQ(v[1], 42);
    EXPECT_EQ(v[2], 42);
    EXPECT_EQ(v[3], 42);
}

TEST(vec_constructor_test, initializer_list) {
    vec3<float> v{1.0f, 2.0f, 3.0f};
    EXPECT_FLOAT_EQ(v[0], 1.0f);
    EXPECT_FLOAT_EQ(v[1], 2.0f);
    EXPECT_FLOAT_EQ(v[2], 3.0f);
}

TEST(vec_constructor_test, initializer_list_partial) {
    vec3<float> v{1.0f, 2.0f};
    EXPECT_FLOAT_EQ(v[0], 1.0f);
    EXPECT_FLOAT_EQ(v[1], 2.0f);
    EXPECT_FLOAT_EQ(v[2], 0.0f);
}

TEST(vec_constructor_test, initializer_list_vec2) {
    vec2<double> v{3.14, 2.71};
    EXPECT_DOUBLE_EQ(v[0], 3.14);
    EXPECT_DOUBLE_EQ(v[1], 2.71);
}

TEST(vec_constructor_test, initializer_list_vec4) {
    vec4<int> v{1, 2, 3, 4};
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
}

TEST(vec_constructor_test, vec_double) {
    vec3<double> v{1.5, 2.5, 3.5};
    EXPECT_DOUBLE_EQ(v[0], 1.5);
    EXPECT_DOUBLE_EQ(v[1], 2.5);
    EXPECT_DOUBLE_EQ(v[2], 3.5);
}

TEST(vec_constructor_test, vec_int) {
    vec3<int> v{1, 2, 3};
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(vec_constructor_test, vec_large_size) {
    vec<float, 100> v(1.0f);
    EXPECT_EQ(v.size(), 100u);
    for (size_t i = 0; i < 100; ++i) {
        EXPECT_FLOAT_EQ(v[i], 1.0f);
    }
}
