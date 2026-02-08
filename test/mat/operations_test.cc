#include <gtest/gtest.h>

#include "xcmath/functions.hpp"
#include "xcmath/mat.hpp"
#include "xcmath/vec.hpp"

using namespace xcmath;

TEST(MatrixOperations, Transpose) {
    mat<float, 2, 3> m = {{1, 2, 3}, {4, 5, 6}};
    auto result = transpose(m);

    EXPECT_EQ(result[0][0], 1);
    EXPECT_EQ(result[0][1], 4);
    EXPECT_EQ(result[1][0], 2);
    EXPECT_EQ(result[1][1], 5);
    EXPECT_EQ(result[2][0], 3);
    EXPECT_EQ(result[2][1], 6);
}

TEST(MatrixOperations, Determinant2x2) {
    mat<float, 2, 2> m = {{1, 2}, {3, 4}};
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, -2.0f);
}

TEST(MatrixOperations, Determinant3x3) {
    mat<float, 3, 3> m = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, 0.0f);
}

TEST(MatrixOperations, Determinant4x4) {
    mat<float, 4, 4> m = {
        {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, 1.0f);
}

TEST(MatrixOperations, Inverse2x2) {
    mat<float, 2, 2> m = {{4, 7}, {2, 6}};
    auto result = inverse(m);

    EXPECT_FLOAT_EQ(result[0][0], 0.6f);
    EXPECT_FLOAT_EQ(result[0][1], -0.7f);
    EXPECT_FLOAT_EQ(result[1][0], -0.2f);
    EXPECT_FLOAT_EQ(result[1][1], 0.4f);
}

TEST(MatrixOperations, Trace) {
    mat<float, 3, 3> m = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    float result = trace(m);
    EXPECT_FLOAT_EQ(result, 15.0f);
}

TEST(MatrixOperations, OuterProduct) {
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

TEST(MatrixOperations, MatrixMultiplication) {
    mat<float, 2, 3> m1 = {{1, 2, 3}, {4, 5, 6}};
    mat<float, 3, 2> m2 = {{7, 8}, {9, 10}, {11, 12}};
    auto result = m1 * m2;

    EXPECT_FLOAT_EQ(result[0][0], 58.0f);
    EXPECT_FLOAT_EQ(result[0][1], 64.0f);
    EXPECT_FLOAT_EQ(result[1][0], 139.0f);
    EXPECT_FLOAT_EQ(result[1][1], 154.0f);
}

TEST(MatrixOperations, MatrixVectorMultiplication) {
    mat<float, 2, 3> m = {{1, 2, 3}, {4, 5, 6}};
    vec<float, 3> v = {7, 8, 9};
    auto result = m * v;

    EXPECT_FLOAT_EQ(result[0], 50.0f);
    EXPECT_FLOAT_EQ(result[1], 122.0f);
}

TEST(MatrixOperations, MatrixAddition) {
    mat<float, 2, 2> m1 = {{1, 2}, {3, 4}};
    mat<float, 2, 2> m2 = {{5, 6}, {7, 8}};
    auto result = m1 + m2;

    EXPECT_FLOAT_EQ(result[0][0], 6.0f);
    EXPECT_FLOAT_EQ(result[0][1], 8.0f);
    EXPECT_FLOAT_EQ(result[1][0], 10.0f);
    EXPECT_FLOAT_EQ(result[1][1], 12.0f);
}

TEST(MatrixOperations, MatrixSubtraction) {
    mat<float, 2, 2> m1 = {{5, 6}, {7, 8}};
    mat<float, 2, 2> m2 = {{1, 2}, {3, 4}};
    auto result = m1 - m2;

    EXPECT_FLOAT_EQ(result[0][0], 4.0f);
    EXPECT_FLOAT_EQ(result[0][1], 4.0f);
    EXPECT_FLOAT_EQ(result[1][0], 4.0f);
    EXPECT_FLOAT_EQ(result[1][1], 4.0f);
}

TEST(MatrixOperations, MatrixScalarMultiplication) {
    mat<float, 2, 2> m = {{1, 2}, {3, 4}};
    auto result = m * 2.0f;

    EXPECT_FLOAT_EQ(result[0][0], 2.0f);
    EXPECT_FLOAT_EQ(result[0][1], 4.0f);
    EXPECT_FLOAT_EQ(result[1][0], 6.0f);
    EXPECT_FLOAT_EQ(result[1][1], 8.0f);
}

TEST(MatrixOperations, ScalarMatrixMultiplication) {
    mat<float, 2, 2> m = {{1, 2}, {3, 4}};
    auto result = 2.0f * m;

    EXPECT_FLOAT_EQ(result[0][0], 2.0f);
    EXPECT_FLOAT_EQ(result[0][1], 4.0f);
    EXPECT_FLOAT_EQ(result[1][0], 6.0f);
    EXPECT_FLOAT_EQ(result[1][1], 8.0f);
}

// Generic determinant tests for larger matrices
TEST(MatrixOperations, Determinant5x5) {
    // Diagonal matrix with values 1, 2, 3, 4, 5
    mat<float, 5, 5> m;
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            m[i, j] = (i == j) ? static_cast<float>(i + 1) : 0.0f;
        }
    }
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, 120.0f);  // 1*2*3*4*5
}

TEST(MatrixOperations, Determinant5x5NonSingular) {
    mat<float, 5, 5> m = {{3, 2, 0, 1, 4},
                           {2, 3, 1, 0, 1},
                           {4, 1, 2, 3, 2},
                           {1, 2, 3, 2, 1},
                           {0, 1, 1, 2, 3}};
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, -56.0f);
}

TEST(MatrixOperations, Determinant6x6UpperTriangular) {
    // Upper triangular matrix with 2s on diagonal
    mat<float, 6, 6> m;
    for (size_t i = 0; i < 6; ++i) {
        for (size_t j = 0; j < 6; ++j) {
            if (i <= j) {
                m[i, j] = 2.0f;
            } else {
                m[i, j] = 0.0f;
            }
        }
    }
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, 64.0f);  // 2^6 = 64
}

TEST(MatrixOperations, Determinant6x6NonSingular) {
    mat<float, 6, 6> m = {{1, 2, 3, 4, 5, 6},
                           {0, 1, 2, 3, 4, 5},
                           {0, 0, 1, 2, 3, 4},
                           {0, 0, 0, 1, 2, 3},
                           {0, 0, 0, 0, 1, 2},
                           {0, 0, 0, 0, 0, 1}};
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, 1.0f);  // Upper triangular with 1s on diagonal
}

TEST(MatrixOperations, DeterminantWithRowSwap) {
    // This matrix has a row that should trigger row swapping
    mat<float, 4, 4> m = {{0, 1, 2, 3},
                           {1, 2, 3, 4},
                           {2, 3, 4, 5},
                           {3, 4, 5, 6}};
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, 0.0f);  // Rows are linearly dependent
}

TEST(MatrixOperations, DeterminantNegative) {
    // Matrix with negative determinant
    mat<float, 3, 3> m = {{2, 1, -1},
                           {-3, -1, 2},
                           {-2, 1, 2}};
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, -1.0f);
}

TEST(MatrixOperations, DeterminantDouble) {
    mat<double, 3, 3> m = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    double result = determinant(m);
    EXPECT_DOUBLE_EQ(result, 0.0);
}

TEST(MatrixOperations, DeterminantIdentity) {
    mat<float, 5, 5> m = {{1, 0, 0, 0, 0},
                           {0, 1, 0, 0, 0},
                           {0, 0, 1, 0, 0},
                           {0, 0, 0, 1, 0},
                           {0, 0, 0, 0, 1}};
    float result = determinant(m);
    EXPECT_FLOAT_EQ(result, 1.0f);
}
