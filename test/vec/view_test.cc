#include "xcmath/vec.hpp"

#include <gtest/gtest.h>

using namespace xcmath;

// Vec View Tests

TEST(vec_view_test, construct_from_pointer) {
    float data[] = {1.0f, 2.0f, 3.0f};
    vec_view<float, 3, 1> view(data);
    EXPECT_FLOAT_EQ(view[0], 1.0f);
    EXPECT_FLOAT_EQ(view[1], 2.0f);
    EXPECT_FLOAT_EQ(view[2], 3.0f);
}

TEST(vec_view_test, modify_through_view) {
    float data[] = {1.0f, 2.0f, 3.0f};
    vec_view<float, 3, 1> view(data);
    view[0] = 10.0f;
    view[1] = 20.0f;
    EXPECT_FLOAT_EQ(data[0], 10.0f);
    EXPECT_FLOAT_EQ(data[1], 20.0f);
}

TEST(vec_view_test, stride_access) {
    float data[] = {1.0f, 0.0f, 2.0f, 0.0f, 3.0f, 0.0f};
    vec_view<float, 3, 2> view(data);
    EXPECT_FLOAT_EQ(view[0], 1.0f);
    EXPECT_FLOAT_EQ(view[1], 2.0f);
    EXPECT_FLOAT_EQ(view[2], 3.0f);
}

TEST(vec_view_test, size_method) {
    float data[] = {1.0f, 2.0f, 3.0f, 4.0f};
    vec_view<float, 4, 1> view(data);
    EXPECT_EQ(view.size(), 4u);
}

TEST(vec_view_test, vec_view_to_const_vec_view_conversion) {
    float data[] = {1.0f, 2.0f, 3.0f};
    vec_view<float, 3, 1> view(data);
    const_vec_view<float, 3, 1> const_view = view;
    EXPECT_FLOAT_EQ(const_view[0], 1.0f);
    EXPECT_FLOAT_EQ(const_view[1], 2.0f);
    EXPECT_FLOAT_EQ(const_view[2], 3.0f);
}

TEST(vec_view_test, assign_from_const_vec_view) {
    float src_data[] = {1.0f, 2.0f, 3.0f};
    float dst_data[] = {0.0f, 0.0f, 0.0f};
    const_vec_view<float, 3, 1> src(src_data);
    vec_view<float, 3, 1> dst(dst_data);
    dst = src;
    EXPECT_FLOAT_EQ(dst_data[0], 1.0f);
    EXPECT_FLOAT_EQ(dst_data[1], 2.0f);
    EXPECT_FLOAT_EQ(dst_data[2], 3.0f);
}

// NOTE: vec_view point accessor test skipped - vec_view does not inherit
// point_accesser

// Const Vec View Tests

TEST(const_vec_view_test, construct_from_pointer) {
    const float data[] = {1.0f, 2.0f, 3.0f};
    const_vec_view<float, 3, 1> view(data);
    EXPECT_FLOAT_EQ(view[0], 1.0f);
    EXPECT_FLOAT_EQ(view[1], 2.0f);
    EXPECT_FLOAT_EQ(view[2], 3.0f);
}

TEST(const_vec_view_test, stride_access) {
    const float data[] = {1.0f, 0.0f, 2.0f, 0.0f, 3.0f};
    const_vec_view<float, 3, 2> view(data);
    EXPECT_FLOAT_EQ(view[0], 1.0f);
    EXPECT_FLOAT_EQ(view[1], 2.0f);
    EXPECT_FLOAT_EQ(view[2], 3.0f);
}

TEST(const_vec_view_test, size_method) {
    const float data[] = {1.0f, 2.0f};
    const_vec_view<float, 2, 1> view(data);
    EXPECT_EQ(view.size(), 2u);
}

// Type Conversion Tests

TEST(vec_conversion_test, vec_to_vec_view) {
    vec3<float> v{1.0f, 2.0f, 3.0f};
    vec_view<float, 3, 1> view = v;
    EXPECT_FLOAT_EQ(view[0], 1.0f);
    EXPECT_FLOAT_EQ(view[1], 2.0f);
    EXPECT_FLOAT_EQ(view[2], 3.0f);
}

TEST(vec_conversion_test, vec_to_const_vec_view) {
    const vec3<float> v{1.0f, 2.0f, 3.0f};
    const_vec_view<float, 3, 1> view = v;
    EXPECT_FLOAT_EQ(view[0], 1.0f);
    EXPECT_FLOAT_EQ(view[1], 2.0f);
    EXPECT_FLOAT_EQ(view[2], 3.0f);
}
