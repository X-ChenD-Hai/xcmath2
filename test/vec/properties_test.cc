#include <gtest/gtest.h>

#include "xcmath/vec.hpp"

using namespace xcmath;

// Vec Properties Tests

TEST(vec_properties_test, dim_basic_type) {
    EXPECT_EQ(vec_properties<float>::dim, 0u);
    EXPECT_EQ(vec_properties<int>::dim, 0u);
    EXPECT_EQ(vec_properties<double>::dim, 0u);
}

TEST(vec_properties_test, dim_vec) {
    EXPECT_EQ(vec_properties<vec3<float>>::dim, 1u);
    EXPECT_EQ(vec_properties<vec4<double>>::dim, 1u);
}

TEST(vec_properties_test, data_type_basic) {
    static_assert(std::is_same_v<vec_properties<float>::data_type, float>);
    static_assert(std::is_same_v<vec_properties<int>::data_type, int>);
}

TEST(vec_properties_test, data_type_vec) {
    static_assert(
        std::is_same_v<vec_properties<vec3<float>>::data_type, float>);
    static_assert(
        std::is_same_v<vec_properties<vec4<double>>::data_type, double>);
}

// Number Properties Tests

TEST(number_properties_test, vec_zero) {
    auto zero = number_meta::number_properties<vec3<float>>::zero;
    EXPECT_FLOAT_EQ(zero[0], 0.0f);
    EXPECT_FLOAT_EQ(zero[1], 0.0f);
    EXPECT_FLOAT_EQ(zero[2], 0.0f);
}
