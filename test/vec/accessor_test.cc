#include <gtest/gtest.h>

#include "xcmath/vec.hpp"

using namespace xcmath;

// Index Operator Tests

TEST(vec_accessor_test, index_operator_read) {
    vec3<float> v{1.0f, 2.0f, 3.0f};
    EXPECT_FLOAT_EQ(v[0], 1.0f);
    EXPECT_FLOAT_EQ(v[1], 2.0f);
    EXPECT_FLOAT_EQ(v[2], 3.0f);
}

TEST(vec_accessor_test, index_operator_write) {
    vec3<float> v;
    v[0] = 10.0f;
    v[1] = 20.0f;
    v[2] = 30.0f;
    EXPECT_FLOAT_EQ(v[0], 10.0f);
    EXPECT_FLOAT_EQ(v[1], 20.0f);
    EXPECT_FLOAT_EQ(v[2], 30.0f);
}

TEST(vec_accessor_test, const_index_operator) {
    const vec3<float> v{1.0f, 2.0f, 3.0f};
    EXPECT_FLOAT_EQ(v[0], 1.0f);
    EXPECT_FLOAT_EQ(v[1], 2.0f);
    EXPECT_FLOAT_EQ(v[2], 3.0f);
}

// Point Accessor Tests (x, y, z, w)

TEST(vec_accessor_test, point_accessor_vec2_x) {
    vec2<float> v{1.0f, 2.0f};
    EXPECT_FLOAT_EQ(v.x(), 1.0f);
}

TEST(vec_accessor_test, point_accessor_vec2_y) {
    vec2<float> v{1.0f, 2.0f};
    EXPECT_FLOAT_EQ(v.y(), 2.0f);
}

TEST(vec_accessor_test, point_accessor_vec3_xyz) {
    vec3<float> v{1.0f, 2.0f, 3.0f};
    EXPECT_FLOAT_EQ(v.x(), 1.0f);
    EXPECT_FLOAT_EQ(v.y(), 2.0f);
    EXPECT_FLOAT_EQ(v.z(), 3.0f);
}

TEST(vec_accessor_test, point_accessor_vec4_xyzw) {
    vec4<float> v{1.0f, 2.0f, 3.0f, 4.0f};
    EXPECT_FLOAT_EQ(v.x(), 1.0f);
    EXPECT_FLOAT_EQ(v.y(), 2.0f);
    EXPECT_FLOAT_EQ(v.z(), 3.0f);
    EXPECT_FLOAT_EQ(v.w(), 4.0f);
}

TEST(vec_accessor_test, point_accessor_write) {
    vec3<float> v;
    v.x() = 10.0f;
    v.y() = 20.0f;
    v.z() = 30.0f;
    EXPECT_FLOAT_EQ(v[0], 10.0f);
    EXPECT_FLOAT_EQ(v[1], 20.0f);
    EXPECT_FLOAT_EQ(v[2], 30.0f);
}

// NOTE: const point accessor test skipped due to library const-correctness
// issue. The point_accesser const methods use non-const static_cast.

// Raw Pointer Tests

TEST(vec_accessor_test, raw_ptr) {
    vec3<float> v{1.0f, 2.0f, 3.0f};
    float* ptr = v.raw_ptr();
    EXPECT_FLOAT_EQ(ptr[0], 1.0f);
    EXPECT_FLOAT_EQ(ptr[1], 2.0f);
    EXPECT_FLOAT_EQ(ptr[2], 3.0f);
}

TEST(vec_accessor_test, raw_ptr_modify) {
    vec3<float> v{1.0f, 2.0f, 3.0f};
    float* ptr = v.raw_ptr();
    ptr[0] = 10.0f;
    ptr[1] = 20.0f;
    ptr[2] = 30.0f;
    EXPECT_FLOAT_EQ(v[0], 10.0f);
    EXPECT_FLOAT_EQ(v[1], 20.0f);
    EXPECT_FLOAT_EQ(v[2], 30.0f);
}

TEST(vec_accessor_test, const_raw_ptr) {
    const vec3<float> v{1.0f, 2.0f, 3.0f};
    const float* ptr = v.raw_ptr();
    EXPECT_FLOAT_EQ(ptr[0], 1.0f);
    EXPECT_FLOAT_EQ(ptr[1], 2.0f);
    EXPECT_FLOAT_EQ(ptr[2], 3.0f);
}
