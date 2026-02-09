#pragma once
#include <cmath>
#include <cstddef>

#include "./alias.hpp"
#include "number_meta.hpp"

namespace xcmath {

template <typename T>
inline constexpr T radians(T degrees) {
    return degrees * number_meta::constants_set<T>::radians;
};
template <typename T>
inline constexpr T degrees(T radians) {
    return radians * number_meta::constants_set<T>::degree;
}

using std::abs;
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
    return m.transpose();
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
            result.at(i, j) = v1[i] * v2[j];
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
            result.at(i, j) = T{};
            for (size_t k = 0; k < col1_; ++k) {
                result.at(i, j) += m1.at(i, k) * m2.at(k, j);
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
            result.at(i) += m.at(i, j) * v.at(j);
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
            result.at(i, j) = m1.at(i, j) + m2.at(i, j);
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
            result.at(i, j) = m1.at(i, j) - m2.at(i, j);
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
            result.at(i, j) = m.at(i, j) * scalar;
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
template <typename T, size_t row_, size_t col_, bool is_col_major_>
constexpr auto inverse(const mat<T, row_, col_, is_col_major_>& m) {
    return m.inverse();
}
}  // namespace xcmath
