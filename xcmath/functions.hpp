#pragma once

#include <cmath>
namespace xcmath {
using std::sqrt;

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
    T result{};
    for (size_t i = 0; i < size_; ++i) {
        result += m[i, i];
    }
    return result;
}

// 向量外积
template <typename T, size_t size_>
constexpr auto outer_product(const vec<T, size_>& v1, const vec<T, size_>& v2) {
    mat<T, size_, size_, false> result;
    for (size_t i = 0; i < size_; ++i) {
        for (size_t j = 0; j < size_; ++j) {
            result[i, j] = v1[i] * v2[j];
        }
    }
    return result;
}

// 矩阵乘法 (mat * mat)
template <typename T, size_t row1_, size_t col1_, size_t col2_, bool is_col_major_>
constexpr auto operator*(const mat<T, row1_, col1_, is_col_major_>& m1, const mat<T, col1_, col2_, is_col_major_>& m2) {
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
constexpr auto operator*(const mat<T, row_, col_, is_col_major_>& m, const vec<T, col_>& v) {
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
constexpr auto operator+(const mat<T, row_, col_, is_col_major_>& m1, const mat<T, row_, col_, is_col_major_>& m2) {
    mat<T, row_, col_, is_col_major_> result;
    for (size_t i = 0; i < row_; ++i) {
        for (size_t j = 0; j < col_; ++j) {
            result[i, j] = m1[i, j] + m2[i, j];
        }
    }
    return result;
}

template <typename T, size_t row_, size_t col_, bool is_col_major_>
constexpr auto operator-(const mat<T, row_, col_, is_col_major_>& m1, const mat<T, row_, col_, is_col_major_>& m2) {
    mat<T, row_, col_, is_col_major_> result;
    for (size_t i = 0; i < row_; ++i) {
        for (size_t j = 0; j < col_; ++j) {
            result[i, j] = m1[i, j] - m2[i, j];
        }
    }
    return result;
}

// 矩阵标量乘法
template <typename T, size_t row_, size_t col_, bool is_col_major_, typename Scalar>
constexpr auto operator*(const mat<T, row_, col_, is_col_major_>& m, Scalar scalar) {
    mat<T, row_, col_, is_col_major_> result;
    for (size_t i = 0; i < row_; ++i) {
        for (size_t j = 0; j < col_; ++j) {
            result[i, j] = m[i, j] * scalar;
        }
    }
    return result;
}

template <typename T, size_t row_, size_t col_, bool is_col_major_, typename Scalar>
constexpr auto operator*(Scalar scalar, const mat<T, row_, col_, is_col_major_>& m) {
    return m * scalar;
}

// 行列式计算 (2x2)
template <typename T, bool is_col_major_>
constexpr T determinant(const mat<T, 2, 2, is_col_major_>& m) {
    return m[0, 0] * m[1, 1] - m[0, 1] * m[1, 0];
}

// 行列式计算 (3x3)
template <typename T, bool is_col_major_>
constexpr T determinant(const mat<T, 3, 3, is_col_major_>& m) {
    return m[0, 0] * (m[1, 1] * m[2, 2] - m[1, 2] * m[2, 1])
         - m[0, 1] * (m[1, 0] * m[2, 2] - m[1, 2] * m[2, 0])
         + m[0, 2] * (m[1, 0] * m[2, 1] - m[1, 1] * m[2, 0]);
}

// 行列式计算 (4x4)
template <typename T, bool is_col_major_>
constexpr T determinant(const mat<T, 4, 4, is_col_major_>& m) {
    return m[0, 0] * determinant(mat<T, 3, 3, is_col_major_>{
        {m[1, 1], m[1, 2], m[1, 3]},
        {m[2, 1], m[2, 2], m[2, 3]},
        {m[3, 1], m[3, 2], m[3, 3]}
    })
         - m[0, 1] * determinant(mat<T, 3, 3, is_col_major_>{
        {m[1, 0], m[1, 2], m[1, 3]},
        {m[2, 0], m[2, 2], m[2, 3]},
        {m[3, 0], m[3, 2], m[3, 3]}
    })
         + m[0, 2] * determinant(mat<T, 3, 3, is_col_major_>{
        {m[1, 0], m[1, 1], m[1, 3]},
        {m[2, 0], m[2, 1], m[2, 3]},
        {m[3, 0], m[3, 1], m[3, 3]}
    })
         - m[0, 3] * determinant(mat<T, 3, 3, is_col_major_>{
        {m[1, 0], m[1, 1], m[1, 2]},
        {m[2, 0], m[2, 1], m[2, 2]},
        {m[3, 0], m[3, 1], m[3, 2]}
    });
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
    result[0, 0] = determinant(mat<T, 2, 2, is_col_major_>{{m[1, 1], m[1, 2]}, {m[2, 1], m[2, 2]}}) / det;
    result[0, 1] = -determinant(mat<T, 2, 2, is_col_major_>{{m[1, 0], m[1, 2]}, {m[2, 0], m[2, 2]}}) / det;
    result[0, 2] = determinant(mat<T, 2, 2, is_col_major_>{{m[1, 0], m[1, 1]}, {m[2, 0], m[2, 1]}}) / det;
    
    result[1, 0] = -determinant(mat<T, 2, 2, is_col_major_>{{m[0, 1], m[0, 2]}, {m[2, 1], m[2, 2]}}) / det;
    result[1, 1] = determinant(mat<T, 2, 2, is_col_major_>{{m[0, 0], m[0, 2]}, {m[2, 0], m[2, 2]}}) / det;
    result[1, 2] = -determinant(mat<T, 2, 2, is_col_major_>{{m[0, 0], m[0, 1]}, {m[2, 0], m[2, 1]}}) / det;
    
    result[2, 0] = determinant(mat<T, 2, 2, is_col_major_>{{m[0, 1], m[0, 2]}, {m[1, 1], m[1, 2]}}) / det;
    result[2, 1] = -determinant(mat<T, 2, 2, is_col_major_>{{m[0, 0], m[0, 2]}, {m[1, 0], m[1, 2]}}) / det;
    result[2, 2] = determinant(mat<T, 2, 2, is_col_major_>{{m[0, 0], m[0, 1]}, {m[1, 0], m[1, 1]}}) / det;
    
    return result;
}

// 矩阵求逆 (4x4)
template <typename T, bool is_col_major_>
constexpr auto inverse(const mat<T, 4, 4, is_col_major_>& m) {
    // 使用伴随矩阵法求逆，这里简化实现
    T det = determinant(m);
    if (det == T{}) {
        return m; // 奇异矩阵，返回原矩阵
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
            adjugate[j, i] = cofactor; // 伴随矩阵是余子式矩阵的转置
        }
    }
    
    return adjugate * (1 / det);
}

}
