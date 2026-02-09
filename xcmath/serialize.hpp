#pragma once
#include <cstddef>
#include <ostream>
#include <type_traits>

#include "./alias.hpp"
#include "./functions.hpp"
#include "./number_meta.hpp"

template <typename T, size_t size_, size_t stride_>
std::ostream& operator<<(std::ostream& out,
                         const xcmath::const_vec_view<T, size_, stride_> v) {
    out << "[";
    for (size_t i = 0; i < size_; ++i) {
        if constexpr (std::is_floating_point_v<T>) {
            if (xcmath::abs(v[i]) <
                xcmath::number_meta::constants_set<T>::epsilon) {
                out << 0.0;
            } else {
                out << v[i];
            }
        } else {
            out << v[i];
        }
        if (i < size_ - 1) {
            out << ", ";
        }
    }
    out << "]";
    return out;
}
template <typename T, size_t size_, size_t stride_>
std::ostream& operator<<(std::ostream& out,
                         const xcmath::vec_view<T, size_, stride_> v) {
    return (out << ((xcmath::const_vec_view<T, size_, stride_>)v));
}
template <typename T, size_t size_>
std::ostream& operator<<(std::ostream& out, const xcmath::vec<T, size_>& v) {
    return (out << ((xcmath::const_vec_view<T, size_, 1>)v));
}

template <typename T, size_t row_, size_t col_, bool is_col_major_>
std::ostream& operator<<(std::ostream& out,
                         const xcmath::mat<T, row_, col_, is_col_major_>& m) {
    out << "[";
    for (size_t i = 0; i < row_; ++i) {
        if (i > 0) {
            out << " ";
        }
        out << m[i];
        if (i < row_ - 1) {
            out << "\n";
        }
    }
    out << "]";
    return out;
}
