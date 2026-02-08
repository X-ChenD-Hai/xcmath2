#pragma once
#include <cmath>
#include <cstddef>

#include "./alias.hpp"

namespace xcmath {
using std::acos;
using std::asin;
using std::atan;
using std::atan2;
using std::ceil;
using std::cos;
using std::cosh;
using std::exp;
using std::fabs;
using std::floor;
using std::fmod;
using std::frexp;
using std::ldexp;
using std::log;
using std::log10;
using std::modf;
using std::pow;
using std::round;
using std::sin;
using std::sinh;
using std::sqrt;
using std::tan;
using std::tanh;

// 矩阵转置
template <typename T, size_t row_, size_t col_, bool is_col_major_>
constexpr auto transpose(const mat<T, row_, col_, is_col_major_>& m) {
    mat<T, col_, row_, !is_col_major_> result;
    for (size_t i = 0; i < row_; ++i) {
        for (size_t j = 0; j < col_; ++j) {
            result[j, i] = m[i, j];
        }
    }
    return result;
}

// 矩阵的迹
template <typename T, size_t size_, bool is_col_major_>
constexpr T trace(const mat<T, size_, size_, is_col_major_>& m) {
    return m.trace();
}

// 向量外积
template <typename T, size_t size_>
constexpr auto outer_product(const vec<T, size_>& v1, const vec<T, size_>& v2) {
    mat<T, size_, size_> result;
    for (size_t i = 0; i < size_; ++i) {
        for (size_t j = 0; j < size_; ++j) {
            result[i, j] = v1[i] * v2[j];
        }
    }
    return result;
}

// 矩阵乘法 (mat * mat)
template <typename T, size_t row1_, size_t col1_, size_t col2_,
          bool is_col_major_>
constexpr auto operator*(const mat<T, row1_, col1_, is_col_major_>& m1,
                         const mat<T, col1_, col2_, is_col_major_>& m2) {
    mat<T, row1_, col2_, is_col_major_> result;
    for (size_t i = 0; i < row1_; ++i) {
        for (size_t j = 0; j < col2_; ++j) {
            result[i, j] = T{};
            for (size_t k = 0; k < col1_; ++k) {
                result[i, j] += m1[i, k] * m2[k, j];
            }
        }
    }
    return result;
}

// 矩阵与向量乘法 (mat * vec)
template <typename T, size_t row_, size_t col_, bool is_col_major_>
constexpr auto operator*(const mat<T, row_, col_, is_col_major_>& m,
                         const vec<T, col_>& v) {
    vec<T, row_> result;
    for (size_t i = 0; i < row_; ++i) {
        result[i] = T{};
        for (size_t j = 0; j < col_; ++j) {
            result[i] += m[i, j] * v[j];
        }
    }
    return result;
}

// 矩阵加减法
template <typename T, size_t row_, size_t col_, bool is_col_major_>
constexpr auto operator+(const mat<T, row_, col_, is_col_major_>& m1,
                         const mat<T, row_, col_, is_col_major_>& m2) {
    mat<T, row_, col_, is_col_major_> result;
    for (size_t i = 0; i < row_; ++i) {
        for (size_t j = 0; j < col_; ++j) {
            result[i, j] = m1[i, j] + m2[i, j];
        }
    }
    return result;
}

template <typename T, size_t row_, size_t col_, bool is_col_major_>
constexpr auto operator-(const mat<T, row_, col_, is_col_major_>& m1,
                         const mat<T, row_, col_, is_col_major_>& m2) {
    mat<T, row_, col_, is_col_major_> result;
    for (size_t i = 0; i < row_; ++i) {
        for (size_t j = 0; j < col_; ++j) {
            result[i, j] = m1[i, j] - m2[i, j];
        }
    }
    return result;
}

// 矩阵标量乘法
template <typename T, size_t row_, size_t col_, bool is_col_major_,
          typename Scalar>
constexpr auto operator*(const mat<T, row_, col_, is_col_major_>& m,
                         Scalar scalar) {
    mat<T, row_, col_, is_col_major_> result;
    for (size_t i = 0; i < row_; ++i) {
        for (size_t j = 0; j < col_; ++j) {
            result[i, j] = m[i, j] * scalar;
        }
    }
    return result;
}

template <typename T, size_t row_, size_t col_, bool is_col_major_,
          typename Scalar>
constexpr auto operator*(Scalar scalar,
                         const mat<T, row_, col_, is_col_major_>& m) {
    return m * scalar;
}

// 行列式计算 - 委托给成员方法
template <typename T, size_t size_, bool is_col_major_>
constexpr T determinant(const mat<T, size_, size_, is_col_major_>& m) {
    return m.determinant();
}

// 矩阵求逆 (2x2)
template <typename T, bool is_col_major_>
constexpr auto inverse(const mat<T, 2, 2, is_col_major_>& m) {
    T det = determinant(m);
    mat<T, 2, 2, is_col_major_> result;
    result[0, 0] = m[1, 1] / det;
    result[0, 1] = -m[0, 1] / det;
    result[1, 0] = -m[1, 0] / det;
    result[1, 1] = m[0, 0] / det;
    return result;
}

// 矩阵求逆 (3x3)
template <typename T, bool is_col_major_>
constexpr auto inverse(const mat<T, 3, 3, is_col_major_>& m) {
    T det = determinant(m);
    mat<T, 3, 3, is_col_major_> result;

    // 伴随矩阵
    result[0, 0] = determinant(mat<T, 2, 2, is_col_major_>{
                       {m[1, 1], m[1, 2]}, {m[2, 1], m[2, 2]}}) /
                   det;
    result[0, 1] = -determinant(mat<T, 2, 2, is_col_major_>{
                       {m[1, 0], m[1, 2]}, {m[2, 0], m[2, 2]}}) /
                   det;
    result[0, 2] = determinant(mat<T, 2, 2, is_col_major_>{
                       {m[1, 0], m[1, 1]}, {m[2, 0], m[2, 1]}}) /
                   det;

    result[1, 0] = -determinant(mat<T, 2, 2, is_col_major_>{
                       {m[0, 1], m[0, 2]}, {m[2, 1], m[2, 2]}}) /
                   det;
    result[1, 1] = determinant(mat<T, 2, 2, is_col_major_>{
                       {m[0, 0], m[0, 2]}, {m[2, 0], m[2, 2]}}) /
                   det;
    result[1, 2] = -determinant(mat<T, 2, 2, is_col_major_>{
                       {m[0, 0], m[0, 1]}, {m[2, 0], m[2, 1]}}) /
                   det;

    result[2, 0] = determinant(mat<T, 2, 2, is_col_major_>{
                       {m[0, 1], m[0, 2]}, {m[1, 1], m[1, 2]}}) /
                   det;
    result[2, 1] = -determinant(mat<T, 2, 2, is_col_major_>{
                       {m[0, 0], m[0, 2]}, {m[1, 0], m[1, 2]}}) /
                   det;
    result[2, 2] = determinant(mat<T, 2, 2, is_col_major_>{
                       {m[0, 0], m[0, 1]}, {m[1, 0], m[1, 1]}}) /
                   det;

    return result;
}

// 矩阵求逆 (4x4)
template <typename T, bool is_col_major_>
constexpr auto inverse(const mat<T, 4, 4, is_col_major_>& m) {
    // 使用伴随矩阵法求逆，这里简化实现
    T det = determinant(m);
    if (det == T{}) {
        return m;  // 奇异矩阵，返回原矩阵
    }

    // 构造伴随矩阵
    mat<T, 4, 4, is_col_major_> adjugate;

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            // 计算代数余子式
            mat<T, 3, 3, is_col_major_> minor;
            size_t minor_row = 0;
            for (size_t r = 0; r < 4; ++r) {
                if (r == i) continue;
                size_t minor_col = 0;
                for (size_t c = 0; c < 4; ++c) {
                    if (c == j) continue;
                    minor[minor_row, minor_col] = m[r, c];
                    minor_col++;
                }
                minor_row++;
            }
            T cofactor = determinant(minor) * ((i + j) % 2 == 0 ? 1 : -1);
            adjugate[j, i] = cofactor;  // 伴随矩阵是余子式矩阵的转置
        }
    }

    return adjugate * (1 / det);
}

// 矩阵求逆 (通用尺寸) - 使用高斯消元法
template <typename T, size_t size_, bool is_col_major_>
constexpr auto inverse(const mat<T, size_, size_, is_col_major_>& m) {
    mat<T, size_, size_, is_col_major_> a = m;
    mat<T, size_, size_, is_col_major_> aug{};

    // Build augmented matrix [A|I]
    for (size_t i = 0; i < size_; ++i) {
        for (size_t j = 0; j < size_; ++j) {
            aug[i, j] = a[i, j];
            aug[i, j + size_] = (i == j) ? T{1} : T{};
        }
    }

    // Gaussian elimination with partial pivoting
    for (size_t i = 0; i < size_; ++i) {
        // Find pivot
        size_t pivot = i;
        for (size_t r = i + 1; r < size_; ++r) {
            if (std::fabs(aug[r, i]) > std::fabs(aug[pivot, i])) {
                pivot = r;
            }
        }

        // Swap rows if needed
        if (pivot != i) {
            for (size_t c = i; c < 2 * size_; ++c) {
                std::swap(aug[i, c], aug[pivot, c]);
            }
        }

        // Check for singular matrix
        if (std::fabs(aug[i, i]) == T{}) {
            return m;  // Singular matrix, return original
        }

        // Scale pivot row
        T pivot_val = aug[i, i];
        for (size_t c = i; c < 2 * size_; ++c) {
            aug[i, c] /= pivot_val;
        }

        // Eliminate column
        for (size_t r = 0; r < size_; ++r) {
            if (r == i) continue;
            T factor = aug[r, i];
            for (size_t c = i; c < 2 * size_; ++c) {
                aug[r, c] -= factor * aug[i, c];
            }
        }
    }

    // Extract inverse from augmented matrix
    mat<T, size_, size_, is_col_major_> result{};
    for (size_t i = 0; i < size_; ++i) {
        for (size_t j = 0; j < size_; ++j) {
            result[i, j] = aug[i, j + size_];
        }
    }

    return result;
}

}  // namespace xcmath
