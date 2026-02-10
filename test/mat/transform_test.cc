#include <gtest/gtest.h>

#include "xcmath/functions.hpp"
#include "xcmath/mat.hpp"
#include "xcmath/mat_factories.hpp"      // IWYU pragma: keep
#include "xcmath/transform_methods.hpp"  // IWYU pragma: keep
#include "xcmath/vec.hpp"

using namespace xcmath;

// ============================================================================
// Rotation Tests (rotate_x, rotate_y, rotate_z)
// ============================================================================

TEST(RotationMethods, RotateX90Degrees) {
    mat<float, 3, 3> m = mat<float, 3, 3>::unit();
    auto result = m.rotate_x(90.0f);

    // Expected rotation matrix for 90 degrees around X:
    // [1,  0,  0]
    // [0,  0, -1]
    // [0,  1,  0]
    EXPECT_NEAR(result[0][0], 1.0f, 1e-6f);
    EXPECT_NEAR(result[0][1], 0.0f, 1e-6f);
    EXPECT_NEAR(result[0][2], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1][0], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1][1], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1][2], -1.0f, 1e-6f);
    EXPECT_NEAR(result[2][0], 0.0f, 1e-6f);
    EXPECT_NEAR(result[2][1], 1.0f, 1e-6f);
    EXPECT_NEAR(result[2][2], 0.0f, 1e-6f);
}

TEST(RotationMethods, RotateX45Degrees) {
    mat<float, 3, 3> m = mat<float, 3, 3>::unit();
    auto result = m.rotate_x(45.0f);

    float rad = xcmath::radians(45.0f);
    float c = xcmath::cos(rad);
    float s = xcmath::sin(rad);

    EXPECT_FLOAT_EQ(result[0][0], 1.0f);
    EXPECT_FLOAT_EQ(result[0][1], 0.0f);
    EXPECT_FLOAT_EQ(result[0][2], 0.0f);
    EXPECT_FLOAT_EQ(result[1][0], 0.0f);
    EXPECT_FLOAT_EQ(result[1][1], c);
    EXPECT_FLOAT_EQ(result[1][2], -s);
    EXPECT_FLOAT_EQ(result[2][0], 0.0f);
    EXPECT_FLOAT_EQ(result[2][1], s);
    EXPECT_FLOAT_EQ(result[2][2], c);
}

TEST(RotationMethods, RotateX4x4) {
    mat<float, 4, 4> m = mat<float, 4, 4>::unit();
    m.at(2, 3) = 5.0f;  // Add Z translation (at column 3, row 2)
    auto result = m.rotate_x(90.0f);

    // Rotation should affect rotation part and translation
    EXPECT_NEAR(result[0][0], 1.0f, 1e-6f);
    EXPECT_NEAR(result[1][1], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1][2], -1.0f, 1e-6f);
    EXPECT_NEAR(result[2][1], 1.0f, 1e-6f);
    EXPECT_NEAR(result[2][2], 0.0f, 1e-6f);
    // Translation should be rotated: Y becomes -Z, Z becomes Y
    // Using rotation formula for X-axis: y' = -z, z' = y
    // If original translation is at (2,3)=5 (Z component = 5), after
    // rotate_x(90): new_y = -5, new_z = 0
    EXPECT_NEAR(result[1][3], -5.0f, 1e-5f);
    EXPECT_NEAR(result[2][3], 0.0f, 1e-5f);
}

TEST(RotationMethods, RotateY90Degrees) {
    mat<float, 3, 3> m = mat<float, 3, 3>::unit();
    auto result = m.rotate_y(90.0f);

    // Expected rotation matrix for 90 degrees around Y:
    // [0,  0,  1]
    // [0,  1,  0]
    // [-1, 0,  0]
    EXPECT_NEAR(result[0][0], 0.0f, 1e-6f);
    EXPECT_NEAR(result[0][1], 0.0f, 1e-6f);
    EXPECT_NEAR(result[0][2], 1.0f, 1e-6f);
    EXPECT_NEAR(result[1][0], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1][1], 1.0f, 1e-6f);
    EXPECT_NEAR(result[1][2], 0.0f, 1e-6f);
    EXPECT_NEAR(result[2][0], -1.0f, 1e-6f);
    EXPECT_NEAR(result[2][1], 0.0f, 1e-6f);
    EXPECT_NEAR(result[2][2], 0.0f, 1e-6f);
}

TEST(RotationMethods, RotateY45Degrees) {
    mat<float, 3, 3> m = mat<float, 3, 3>::unit();
    auto result = m.rotate_y(45.0f);

    float rad = xcmath::radians(45.0f);
    float c = xcmath::cos(rad);
    float s = xcmath::sin(rad);

    EXPECT_FLOAT_EQ(result[0][0], c);
    EXPECT_FLOAT_EQ(result[0][1], 0.0f);
    EXPECT_FLOAT_EQ(result[0][2], s);
    EXPECT_FLOAT_EQ(result[1][0], 0.0f);
    EXPECT_FLOAT_EQ(result[1][1], 1.0f);
    EXPECT_FLOAT_EQ(result[1][2], 0.0f);
    EXPECT_FLOAT_EQ(result[2][0], -s);
    EXPECT_FLOAT_EQ(result[2][1], 0.0f);
    EXPECT_FLOAT_EQ(result[2][2], c);
}

TEST(RotationMethods, RotateZ90Degrees) {
    mat<float, 3, 3> m = mat<float, 3, 3>::unit();
    auto result = m.rotate_z(90.0f);

    // Expected rotation matrix for 90 degrees around Z:
    // [0, -1,  0]
    // [1,  0,  0]
    // [0,  0,  1]
    EXPECT_NEAR(result[0][0], 0.0f, 1e-6f);
    EXPECT_NEAR(result[0][1], -1.0f, 1e-6f);
    EXPECT_NEAR(result[0][2], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1][0], 1.0f, 1e-6f);
    EXPECT_NEAR(result[1][1], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1][2], 0.0f, 1e-6f);
    EXPECT_NEAR(result[2][0], 0.0f, 1e-6f);
    EXPECT_NEAR(result[2][1], 0.0f, 1e-6f);
    EXPECT_NEAR(result[2][2], 1.0f, 1e-6f);
}

TEST(RotationMethods, RotateZ45Degrees) {
    mat<float, 3, 3> m = mat<float, 3, 3>::unit();
    auto result = m.rotate_z(45.0f);

    float rad = xcmath::radians(45.0f);
    float c = xcmath::cos(rad);
    float s = xcmath::sin(rad);

    EXPECT_FLOAT_EQ(result[0][0], c);
    EXPECT_FLOAT_EQ(result[0][1], -s);
    EXPECT_FLOAT_EQ(result[1][0], s);
    EXPECT_FLOAT_EQ(result[1][1], c);
    EXPECT_FLOAT_EQ(result[2][0], 0.0f);
    EXPECT_FLOAT_EQ(result[2][1], 0.0f);
    EXPECT_FLOAT_EQ(result[2][2], 1.0f);
}

TEST(RotationMethods, RotateZ180Degrees) {
    mat<float, 3, 3> m = mat<float, 3, 3>::unit();
    auto result = m.rotate_z(180.0f);

    // 180 degree rotation around Z should negate X and Y
    EXPECT_NEAR(result[0][0], -1.0f, 1e-6f);
    EXPECT_NEAR(result[0][1], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1][0], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1][1], -1.0f, 1e-6f);
    EXPECT_NEAR(result[2][0], 0.0f, 1e-6f);
    EXPECT_NEAR(result[2][1], 0.0f, 1e-6f);
    EXPECT_NEAR(result[2][2], 1.0f, 1e-6f);
}

// ============================================================================
// Shear Tests
// ============================================================================

TEST(ShearMethods, ShearXY) {
    mat<float, 3, 3> m = mat<float, 3, 3>::unit();
    vec<float, 6> factors = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    auto result = m.shear(factors);

    // [1, 1, 0]
    // [0, 1, 0]
    // [0, 0, 1]
    EXPECT_FLOAT_EQ(result[0][0], 1.0f);
    EXPECT_FLOAT_EQ(result[0][1], 1.0f);
    EXPECT_FLOAT_EQ(result[0][2], 0.0f);
    EXPECT_FLOAT_EQ(result[1][0], 0.0f);
    EXPECT_FLOAT_EQ(result[1][1], 1.0f);
    EXPECT_FLOAT_EQ(result[2][0], 0.0f);
    EXPECT_FLOAT_EQ(result[2][1], 0.0f);
    EXPECT_FLOAT_EQ(result[2][2], 1.0f);
}

TEST(ShearMethods, ShearXZAndYZ) {
    mat<float, 3, 3> m = mat<float, 3, 3>::unit();
    vec<float, 6> factors = {0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0.0f};
    auto result = m.shear(factors);

    // [1, 0, 1]
    // [0, 1, 0.5]
    // [0, 0, 1]
    EXPECT_FLOAT_EQ(result[0][0], 1.0f);
    EXPECT_FLOAT_EQ(result[0][1], 0.0f);
    EXPECT_FLOAT_EQ(result[0][2], 1.0f);
    EXPECT_FLOAT_EQ(result[1][0], 0.0f);
    EXPECT_FLOAT_EQ(result[1][1], 1.0f);
    EXPECT_FLOAT_EQ(result[1][2], 0.5f);
    EXPECT_FLOAT_EQ(result[2][0], 0.0f);
    EXPECT_FLOAT_EQ(result[2][1], 0.0f);
    EXPECT_FLOAT_EQ(result[2][2], 1.0f);
}

TEST(ShearMethods, Shear4x4) {
    mat<float, 4, 4> m = mat<float, 4, 4>::unit();
    vec<float, 6> factors = {0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    auto result = m.shear(factors);

    // 4x4 shear should only affect upper 3x3 rotation part
    EXPECT_FLOAT_EQ(result[0][0], 1.0f);
    EXPECT_FLOAT_EQ(result[0][1], 0.5f);
    EXPECT_FLOAT_EQ(result[0][2], 0.0f);
    EXPECT_FLOAT_EQ(result[1][0], 0.0f);
    EXPECT_FLOAT_EQ(result[1][1], 1.0f);
    EXPECT_FLOAT_EQ(result[3][3], 1.0f);
}

// ============================================================================
// Factory Tests (ortho, frustum)
// ============================================================================

TEST(FactoryMethods, OrthoStandard) {
    // Standard ortho projection: left=-1, right=1, bottom=-1, top=1, near=-1,
    // far=1
    auto result =
        mat<float, 4, 4>::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    // Expected:
    // [1, 0, 0, 0]
    // [0, 1, 0, 0]
    // [0, 0, -1, 0]
    // [0, 0, 0, 1]
    EXPECT_FLOAT_EQ(result[0][0], 1.0f);
    EXPECT_FLOAT_EQ(result[0][1], 0.0f);
    EXPECT_FLOAT_EQ(result[0][2], 0.0f);
    EXPECT_FLOAT_EQ(result[0][3], 0.0f);

    EXPECT_FLOAT_EQ(result[1][0], 0.0f);
    EXPECT_FLOAT_EQ(result[1][1], 1.0f);
    EXPECT_FLOAT_EQ(result[1][2], 0.0f);
    EXPECT_FLOAT_EQ(result[1][3], 0.0f);

    EXPECT_FLOAT_EQ(result[2][0], 0.0f);
    EXPECT_FLOAT_EQ(result[2][1], 0.0f);
    EXPECT_FLOAT_EQ(result[2][2], -1.0f);
    EXPECT_FLOAT_EQ(result[2][3], 0.0f);

    EXPECT_FLOAT_EQ(result[3][0], 0.0f);
    EXPECT_FLOAT_EQ(result[3][1], 0.0f);
    EXPECT_FLOAT_EQ(result[3][2], 0.0f);
    EXPECT_FLOAT_EQ(result[3][3], 1.0f);
}

TEST(FactoryMethods, OrthoCustom) {
    // Ortho with custom bounds: left=0, right=10, bottom=0, top=5, near=-1,
    // far=1
    auto result = mat<float, 4, 4>::ortho(0.0f, 10.0f, 0.0f, 5.0f, -1.0f, 1.0f);

    // Scale: 2/10 = 0.2, 2/5 = 0.4, -2/2 = -1
    // Translation: -(10+0)/10 = -1, -(5+0)/5 = -1, -(1+(-1))/2 = 0
    EXPECT_FLOAT_EQ(result[0][0], 0.2f);
    EXPECT_FLOAT_EQ(result[0][3], -1.0f);
    EXPECT_FLOAT_EQ(result[1][1], 0.4f);
    EXPECT_FLOAT_EQ(result[1][3], -1.0f);
    EXPECT_FLOAT_EQ(result[2][2], -1.0f);
    EXPECT_FLOAT_EQ(result[2][3], 0.0f);
    EXPECT_FLOAT_EQ(result[3][3], 1.0f);
}

TEST(FactoryMethods, FrustumStandard) {
    // Standard frustum: left=-1, right=1, bottom=-1, top=1, near=1, far=10
    auto result =
        mat<float, 4, 4>::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 10.0f);

    // Verify structure
    EXPECT_FLOAT_EQ(result[0][0], 1.0f);   // 2*near/width = 2*1/2 = 1
    EXPECT_FLOAT_EQ(result[1][1], 1.0f);   // 2*near/height = 2*1/2 = 1
    EXPECT_FLOAT_EQ(result[3][2], -1.0f);  // w component should be -1
    EXPECT_FLOAT_EQ(result[2][2],
                    -11.0f / 9.0f);  // -(far+near)/(far-near) = -11/9
}

TEST(FactoryMethods, FrustumAsymmetric) {
    // Asymmetric frustum: left=-2, right=1, bottom=-1, top=1, near=1, far=100
    auto result =
        mat<float, 4, 4>::frustum(-2.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f);

    float rl = 3.0f;   // right - left
    float tb = 2.0f;   // top - bottom
    float fn = 99.0f;  // far - near

    EXPECT_NEAR(result[0][0], 2.0f / rl, 1e-6f);  // 2*near/width
    EXPECT_NEAR(result[1][1], 2.0f / tb, 1e-6f);  // 2*near/height
    EXPECT_NEAR(result[0][2], (-2.0f + 1.0f) / rl,
                1e-6f);  // (right+left)/width = -1/3
    EXPECT_NEAR(result[1][2], (-1.0f + 1.0f) / tb,
                1e-6f);  // (top+bottom)/height = 0
    EXPECT_NEAR(result[2][2], -(100.0f + 1.0f) / fn,
                1e-5f);  // -(far+near)/(far-near)
    EXPECT_NEAR(result[2][3], -(2.0f * 100.0f * 1.0f) / fn,
                1e-5f);  // -2*far*near/(far-near)
    EXPECT_NEAR(result[3][2], -1.0f, 1e-6f);
}

TEST(FactoryMethods, FrustumSymmetric) {
    // Symmetric frustum: left=-width, right=width, bottom=-height, top=height
    auto result =
        mat<float, 4, 4>::frustum(-2.0f, 2.0f, -1.5f, 1.5f, 1.0f, 100.0f);

    // For symmetric frustum, (right+left)/width and (top+bottom)/height should
    // be 0
    EXPECT_FLOAT_EQ(result[0][2], 0.0f);
    EXPECT_FLOAT_EQ(result[1][2], 0.0f);
}

// ============================================================================
// LookAt Tests
// ============================================================================

TEST(FactoryMethods, LookAtOrigin) {
    // Eye at origin, looking at +Z direction
    vec<float, 3> eye = {0.0f, 0.0f, 0.0f};
    vec<float, 3> center = {0.0f, 0.0f, 1.0f};
    vec<float, 3> up = {0.0f, 1.0f, 0.0f};

    auto result = mat<float, 4, 4>::look_at(eye, center, up);

    // Forward = (0, 0, 1), Right = (-1, 0, 0), Up = (0, 1, 0)
    // Z axis is negated for view matrix: -forward = (0, 0, -1)
    EXPECT_FLOAT_EQ(result[0][0], -1.0f);
    EXPECT_FLOAT_EQ(result[0][1], 0.0f);
    EXPECT_FLOAT_EQ(result[0][2], 0.0f);
    EXPECT_FLOAT_EQ(result[1][0], 0.0f);
    EXPECT_FLOAT_EQ(result[1][1], 1.0f);
    EXPECT_FLOAT_EQ(result[1][2], 0.0f);
    EXPECT_FLOAT_EQ(result[2][0], 0.0f);
    EXPECT_FLOAT_EQ(result[2][1], 0.0f);
    EXPECT_FLOAT_EQ(result[2][2], -1.0f);
    EXPECT_FLOAT_EQ(result[3][3], 1.0f);
}

TEST(FactoryMethods, LookAtEyeAtZ5) {
    // Eye at (0, 0, 5), looking at origin
    vec<float, 3> eye = {0.0f, 0.0f, 5.0f};
    vec<float, 3> center = {0.0f, 0.0f, 0.0f};
    vec<float, 3> up = {0.0f, 1.0f, 0.0f};

    auto result = mat<float, 4, 4>::look_at(eye, center, up);

    // Forward = (0, 0, -1), Right = (1, 0, 0), Up = (0, 1, 0)
    EXPECT_NEAR(result[0][0], 1.0f, 1e-6f);
    EXPECT_NEAR(result[0][1], 0.0f, 1e-6f);
    EXPECT_NEAR(result[0][2], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1][0], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1][1], 1.0f, 1e-6f);
    EXPECT_NEAR(result[1][2], 0.0f, 1e-6f);
    EXPECT_NEAR(result[2][0], 0.0f, 1e-6f);
    EXPECT_NEAR(result[2][1], 0.0f, 1e-6f);
    EXPECT_NEAR(result[2][2], 1.0f, 1e-6f);  // -forward.z = -(-1) = 1
}

TEST(FactoryMethods, LookAtWithTranslation) {
    // Eye at (0, 0, 5), looking at origin
    vec<float, 3> eye = {0.0f, 0.0f, 5.0f};
    vec<float, 3> center = {0.0f, 0.0f, 0.0f};
    vec<float, 3> up = {0.0f, 1.0f, 0.0f};

    auto result = mat<float, 4, 4>::look_at(eye, center, up);

    // The translation component should position the camera
    // Result[0][3], result[1][3], result[2][3] should contain the translation
    // that moves the world to align with the camera view
    EXPECT_FLOAT_EQ(result[3][3], 1.0f);
}

// ============================================================================
// Composition Tests (combining transforms)
// ============================================================================

TEST(TransformComposition, TranslateThenRotate) {
    mat<float, 4, 4> m = mat<float, 4, 4>::unit();
    m = m.translate(vec<float, 3>{1.0f, 0.0f, 0.0f});
    m = m.rotate_z(90.0f);

    // For p = (1,0,0,1):
    // R(90) around Z: cos(90)=0, sin(90)=1
    // R(90) * T(1,0,0) should give:
    //   p' = R(90) * p + R(90) * t
    //   = (0*1 + 1*0, 1*1 + 0*0, 0, 1) + (0*1 + 1*0, 1*1 + 0*0, 0, 0)
    //   = (0, 1, 0, 1) + (0, 1, 0, 0) = (0, 2, 0, 1)
    // So result should be (0, 2, 0, 1)
    vec<float, 4> p = {1.0f, 0.0f, 0.0f, 1.0f};
    auto result = m * p;

    EXPECT_NEAR(result[0], 0.0f, 1e-5f);
    EXPECT_NEAR(result[1], 2.0f, 1e-5f);  // 旋转后的平移量
}

TEST(TransformComposition, ScaleThenRotate) {
    mat<float, 4, 4> m = mat<float, 4, 4>::unit();
    m = m.scale(vec<float, 3>{2.0f, 2.0f, 2.0f});
    m = m.rotate_z(90.0f);

    // After scaling by 2 then rotation by 90 deg around Z:
    // Point (1,0,0) should become (0,2,0)
    vec<float, 4> p = {1.0f, 0.0f, 0.0f, 1.0f};
    auto result = m * p;

    EXPECT_NEAR(result[0], 0.0f, 1e-6f);
    EXPECT_NEAR(result[1], 2.0f, 1e-6f);
}

TEST(TransformComposition, RotateXThenRotateY) {
    mat<float, 4, 4> m = mat<float, 4, 4>::unit();
    m = m.rotate_x(90.0f);
    m = m.rotate_y(90.0f);

    // Verify the composition is valid (determinant should be 1 for rotation
    // matrix)
    float det = m.determinant();
    EXPECT_NEAR(det, 1.0f, 1e-6f);
}

// ============================================================================
// Double Precision Tests
// ============================================================================

TEST(PrecisionTests, RotateXDouble) {
    mat<double, 3, 3> m = mat<double, 3, 3>::unit();
    auto result = m.rotate_x(45.0);

    double rad = xcmath::radians(45.0);
    double c = xcmath::cos(rad);
    double s = xcmath::sin(rad);

    EXPECT_DOUBLE_EQ(result[0][0], 1.0);
    EXPECT_DOUBLE_EQ(result[1][1], c);
    EXPECT_DOUBLE_EQ(result[1][2], -s);
    EXPECT_DOUBLE_EQ(result[2][1], s);
    EXPECT_DOUBLE_EQ(result[2][2], c);
}

TEST(PrecisionTests, OrthoDouble) {
    auto result = mat<double, 4, 4>::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    EXPECT_DOUBLE_EQ(result[0][0], 1.0);
    EXPECT_DOUBLE_EQ(result[1][1], 1.0);
    EXPECT_DOUBLE_EQ(result[2][2], -1.0);
    EXPECT_DOUBLE_EQ(result[3][3], 1.0);
}

TEST(PrecisionTests, FrustumDouble) {
    auto result = mat<double, 4, 4>::frustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);

    EXPECT_DOUBLE_EQ(result[0][0], 1.0);
    EXPECT_DOUBLE_EQ(result[1][1], 1.0);
    EXPECT_DOUBLE_EQ(result[3][2], -1.0);
}
