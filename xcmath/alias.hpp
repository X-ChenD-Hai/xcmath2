#pragma once
#include <assert.h>
#include <cstdint>
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

// Float aliases
using vec2f = vec<float, 2>;
using vec3f = vec<float, 3>;
using vec4f = vec<float, 4>;
using mat2f = mat<float, 2, 2>;
using mat3f = mat<float, 3, 3>;
using mat4f = mat<float, 4, 4>;

// Double aliases
using vec2d = vec<double, 2>;
using vec3d = vec<double, 3>;
using vec4d = vec<double, 4>;
using mat2d = mat<double, 2, 2>;
using mat3d = mat<double, 3, 3>;
using mat4d = mat<double, 4, 4>;

// Int32 aliases
using vec2i32 = vec<int32_t, 2>;
using vec3i32 = vec<int32_t, 3>;
using vec4i32 = vec<int32_t, 4>;

// Int64 aliases
using vec2i64 = vec<int64_t, 2>;
using vec3i64 = vec<int64_t, 3>;
using vec4i64 = vec<int64_t, 4>;
}  // namespace xcmath