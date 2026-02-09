#include <gtest/gtest.h>

#include "xcmath/functions.hpp"
#include "xcmath/mat.hpp"
#include "xcmath/vec.hpp"

using namespace xcmath;

// 列优先矩阵测试
TEST(MatrixOperationsColMajor, Transpose) {
    mat<float, 2, 3, true> m{{1, 2, 3}, {4, 5, 6}};
    auto result = transpose(m);

    EXPECT_EQ(result[0][0], 1);
    EXPECT_EQ(result[0][1], 4);
    EXPECT_EQ(result[1][0], 2);
    EXPECT_EQ(result[1][1], 5);
    EXPECT_EQ(result[2][0], 3);
    EXPECT_EQ(result[2][1], 6);
}

TEST(MatrixOperationsColMajor, Determinant2x2) {
    mat<float, 2, 2, true> m = {{1, 2}, {3, 4}};
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, -2.0f);
}

TEST(MatrixOperationsColMajor, Determinant3x3) {
    mat<float, 3, 3, true> m = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, 0.0f);
}

TEST(MatrixOperationsColMajor, Determinant4x4) {
    mat<float, 4, 4, true> m = {
        {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, 1.0f);
}

TEST(MatrixOperationsColMajor, Inverse2x2) {
    mat<float, 2, 2, true> m = {{4, 7}, {2, 6}};
    auto result = inverse(m);

    EXPECT_FLOAT_EQ(result[0][0], 0.6f);
    EXPECT_FLOAT_EQ(result[0][1], -0.7f);
    EXPECT_FLOAT_EQ(result[1][0], -0.2f);
    EXPECT_FLOAT_EQ(result[1][1], 0.4f);
}

TEST(MatrixOperationsColMajor, Trace) {
    mat<float, 3, 3, true> m = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    float result = trace(m);
    EXPECT_FLOAT_EQ(result, 15.0f);
}

TEST(MatrixOperationsColMajor, OuterProduct) {
    vec<float, 3> v1 = {1, 2, 3};
    vec<float, 3> v2 = {4, 5, 6};
    auto result = outer_product(v1, v2);

    EXPECT_FLOAT_EQ(result[0][0], 4.0f);
    EXPECT_FLOAT_EQ(result[0][1], 5.0f);
    EXPECT_FLOAT_EQ(result[0][2], 6.0f);
    EXPECT_FLOAT_EQ(result[1][0], 8.0f);
    EXPECT_FLOAT_EQ(result[1][1], 10.0f);
    EXPECT_FLOAT_EQ(result[1][2], 12.0f);
    EXPECT_FLOAT_EQ(result[2][0], 12.0f);
    EXPECT_FLOAT_EQ(result[2][1], 15.0f);
    EXPECT_FLOAT_EQ(result[2][2], 18.0f);
}

TEST(MatrixOperationsColMajor, MatrixMultiplication) {
    mat<float, 2, 3, true> m1 = {{1, 2, 3}, {4, 5, 6}};
    mat<float, 3, 2, true> m2 = {{7, 8}, {9, 10}, {11, 12}};
    auto result = m1 * m2;

    EXPECT_FLOAT_EQ(result[0][0], 58.0f);
    EXPECT_FLOAT_EQ(result[0][1], 64.0f);
    EXPECT_FLOAT_EQ(result[1][0], 139.0f);
    EXPECT_FLOAT_EQ(result[1][1], 154.0f);
}

TEST(MatrixOperationsColMajor, MatrixVectorMultiplication) {
    mat<float, 2, 3, true> m = {{1, 2, 3}, {4, 5, 6}};
    vec<float, 3> v = {7, 8, 9};
    auto result = m * v;

    EXPECT_FLOAT_EQ(result[0], 50.0f);
    EXPECT_FLOAT_EQ(result[1], 122.0f);
}

TEST(MatrixOperationsColMajor, MatrixAddition) {
    mat<float, 2, 2, true> m1 = {{1, 2}, {3, 4}};
    mat<float, 2, 2, true> m2 = {{5, 6}, {7, 8}};
    auto result = m1 + m2;

    EXPECT_FLOAT_EQ(result[0][0], 6.0f);
    EXPECT_FLOAT_EQ(result[0][1], 8.0f);
    EXPECT_FLOAT_EQ(result[1][0], 10.0f);
    EXPECT_FLOAT_EQ(result[1][1], 12.0f);
}

TEST(MatrixOperationsColMajor, MatrixSubtraction) {
    mat<float, 2, 2, true> m1 = {{5, 6}, {7, 8}};
    mat<float, 2, 2, true> m2 = {{1, 2}, {3, 4}};
    auto result = m1 - m2;

    EXPECT_FLOAT_EQ(result[0][0], 4.0f);
    EXPECT_FLOAT_EQ(result[0][1], 4.0f);
    EXPECT_FLOAT_EQ(result[1][0], 4.0f);
    EXPECT_FLOAT_EQ(result[1][1], 4.0f);
}

TEST(MatrixOperationsColMajor, MatrixScalarMultiplication) {
    mat<float, 2, 2, true> m = {{1, 2}, {3, 4}};
    auto result = m * 2.0f;

    EXPECT_FLOAT_EQ(result[0][0], 2.0f);
    EXPECT_FLOAT_EQ(result[0][1], 4.0f);
    EXPECT_FLOAT_EQ(result[1][0], 6.0f);
    EXPECT_FLOAT_EQ(result[1][1], 8.0f);
}

TEST(MatrixOperationsColMajor, ScalarMatrixMultiplication) {
    mat<float, 2, 2, true> m = {{1, 2}, {3, 4}};
    auto result = 2.0f * m;

    EXPECT_FLOAT_EQ(result[0][0], 2.0f);
    EXPECT_FLOAT_EQ(result[0][1], 4.0f);
    EXPECT_FLOAT_EQ(result[1][0], 6.0f);
    EXPECT_FLOAT_EQ(result[1][1], 8.0f);
}
