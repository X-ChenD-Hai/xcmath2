#pragma once
#include <assert.h>
#define assert_index(idx, size_) assert((idx) < (size_) && "index out of range")

namespace xcmath {
template <typename T, size_t size_>
class vec;
template <typename T, size_t size_, size_t stride_>
class const_vec_view;
template <typename T, size_t size_, size_t stride_>
class vec_view;
#ifdef ENABLE_COL_MAJOR_MAT
template <typename T, size_t row_, size_t col_, bool is_col_major_ = true>
#else
template <typename T, size_t row_, size_t col_, bool is_col_major_ = false>
#endif
class mat;

template <typename T>
using vec2 = vec<T, 2>;
template <typename T>
using vec3 = vec<T, 3>;
template <typename T>
using vec4 = vec<T, 4>;

template <typename T>
using mat2 = mat<T, 2, 2>;
template <typename T>
using mat3 = mat<T, 3, 3>;
template <typename T>
using mat4 = mat<T, 4, 4>;

using mat3f = mat<float, 3, 3>;
}  // namespace xcmath