# xcmath - C++ Linear Algebra Library

[![GitHub License](https://img.shields.io/github/license/xcrtp/xcmath)](https://github.com/xcrtp/xcmath/blob/main/LICENSE)
[![GitHub Release](https://img.shields.io/github/v/release/xcrtp/xcmath)](https://github.com/xcrtp/xcmath/releases)
[![GitHub Tag](https://img.shields.io/github/v/tag/xcrtp/xcmath)](https://github.com/xcrtp/xcmath/tags)
![GitHub top language](https://img.shields.io/github/languages/top/xcrtp/xcmath?style=flat)

[English](./README.md) | [中文](./README-zh.md)

## Project Overview

**xcmath** is a modern C++20 header-only linear algebra library providing `vec<T, N>` and `mat<T, R, C>` types with a CRTP-based method composition framework. Designed for the xcal project.

## Core Features

- **Header-Only**: No build required, just include and use
- **CRTP-Based Methods**: Compile-time method composition without virtual function overhead
- **Type-Safe**: Full compile-time type checking
- **Flexible**: Support for custom numeric types, view types for slicing

## Quick Example

```cpp
#include <xcmath/vec.hpp>
#include <xcmath/mat.hpp>

using namespace xcmath;

// Vector operations
vec3f v1{1, 2, 3};
vec3f v2{4, 5, 6};

float dotProduct = v1.dot(v2);           // Dot product
vec3f crossProduct = v1.cross(v2);       // Cross product
float magnitude = v1.module();          // Length
vec3f normalized = v1.normalize();        // Normalized vector

// Matrix operations
mat4f m = mat4f::unit();                 // Identity matrix
m = m.rotate(45.0f, vec3f{0, 0, 1})     // Rotation
     .scale(2.0f)                        // Scale
     .translate(vec3f{1, 2, 3});        // Translation

// Matrix-vector multiplication
vec4f result = m * vec4f{1, 0, 0, 1};
```

## Vector Types

### Basic Vectors

```cpp
#include <xcmath/vec.hpp>

using namespace xcmath;

// Type aliases (float by default)
vec2f v2;    // 2D vector
vec3f v3;    // 3D vector
vec4f v4;    // 4D vector

// Custom size and type
vec<double, 10> v10;    // 10D vector of double
vec<int, 3> vi3;        // 3D vector of int

// Initialization
vec3f v1{1, 2, 3};               // Brace initialization
vec3f v2 = vec3f::zero();        // Zero vector
vec3f v3 = vec3f::unit();        // Unit vector (1, 1, 1)
```

### Point Accessors

For vectors of size 1-4, you can use point accessors:

```cpp
vec2f v2{1, 2};
float x = v2.x();    // 1
float y = v2.y();    // 2

vec3f v3{1, 2, 3};
float z = v3.z();    // 3

vec4f v4{1, 2, 3, 4};
float w = v4.w();    // 4
```

### Vector Methods

| Method | Description | Example |
|--------|-------------|---------|
| `size()` | Returns vector dimension | `v.size()` → 3 |
| `module()` | Returns vector length | `v.module()` → 5.0 |
| `normalize()` | Returns unit vector | `v.normalize()` |
| `dot(v)` | Dot product | `v1.dot(v2)` |
| `cross(v)` | Cross product (vec3 only) | `v1.cross(v2)` |
| `distance(v)` | Distance to another vector | `v1.distance(v2)` |
| `distance_squared(v)` | Squared distance | `v1.distance_squared(v2)` |
| `angle(v)` | Angle between vectors (radians) | `v1.angle(v2)` |
| `project(v)` | Project onto vector | `v1.project(v2)` |
| `reflect(normal)` | Reflect around normal | `v.reflect(n)` |

### Component-wise Operations

```cpp
vec3f v{-1.5f, 2.8f, 0.0f};

v.abs();        // {1.5, 2.8, 0.0}
v.floor();      // {-2, 2, 0}
v.ceil();       // {-1, 3, 0}
v.round();      // {-2, 3, 0}
v.fract();      // {0.5, 0.8, 0.0}
v.sign();       // {-1, 1, 0}

vec3f a{1, 5, 3};
vec3f b{3, 2, 4};
a.min(b);       // {1, 2, 3}  - component-wise min
a.max(b);       // {3, 5, 4}  - component-wise max
a.clamp(lo, hi);// component-wise clamp
```

### Comparison Operations

```cpp
vec3f v1{1, 2, 3};
vec3f v2{1, 2, 4};

v1 == v2;       // Element-wise equality
v1 != v2;       // Element-wise inequality

v1.equal(v2, epsilon);  // Compare with tolerance
v1.less_than(v2);       // Element-wise comparison (returns vec)
v1.greater_than(v2);    // Element-wise comparison (returns vec)
v1.any();                // Any element is non-zero
v1.all();                // All elements are non-zero
```

## Matrix Types

### Basic Matrices

```cpp
#include <xcmath/mat.hpp>

using namespace xcmath;

// Type aliases (row-major by default)
mat2f m2;    // 2x2 matrix
mat3f m3;    // 3x3 matrix
mat4f m4;    // 4x4 matrix

// Custom size and type
mat<double, 3, 4> m;    // 3x4 matrix

// Initialization
mat3f m1{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};  // Row-major
mat3f m2 = mat3f::unit();                    // Identity matrix
mat3f m3 = mat3f::zero();                    // Zero matrix
```

### Column-Major Matrices

Define `ENABLE_COL_MAJOR_MAT` before including headers:

```cpp
#define ENABLE_COL_MAJOR_MAT
#include <xcmath/mat.hpp>

using namespace xcmath;
mat4f colMajor = mat4f::unit();  // Now column-major
```

### Matrix Methods

| Method | Description |
|--------|-------------|
| `transpose()` | Returns transposed matrix |
| `determinant()` | Returns determinant (2x2, 3x3, 4x4) |
| `inverse()` | Returns inverse matrix |
| `trace()` | Returns sum of diagonal elements |
| `rotate(angle, axis)` | Returns rotation matrix |
| `rotate_x/y/z(angle)` | Returns rotation around axis |
| `scale(s)` | Returns scaled matrix |
| `translate(v)` | Returns translation matrix |
| `look_at(eye, center, up)` | Returns view matrix |

### Matrix Operations

```cpp
mat3f m1{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
mat3f m2 = mat3f::unit();

mat3f sum = m1 + m2;                     // Matrix addition
mat3f diff = m1 - m2;                    // Matrix subtraction
mat3f prod = m1 * m2;                    // Matrix multiplication
vec3f v = m1 * vec3f{1, 2, 3};           // Matrix-vector multiplication

// Free functions
mat3f t = transpose(m1);                 // Transpose
float d = determinant(m1);               // Determinant
mat3f i = inverse(m1);                   // Inverse
float tr = trace(m1);                     // Trace
mat3f outer = outer_product(v1, v2);     // Outer product
```

## Transformations

```cpp
#include <xcmath/transform_methods.hpp>

mat4f transform = mat4f::unit()
    .rotate(45.0f, vec3f{0, 0, 1})   // Rotate 45° around Z
    .scale(2.0f)                     // Scale 2x
    .translate(vec3f{1, 2, 3});      // Translate

vec4f result = transform * vec4f{1, 0, 0, 1};
```

## Interpolation

```cpp
#include <xcmath/interpolation.hpp>

using namespace xcmath;

vec3f a{0, 0, 0};
vec3f b{10, 10, 10};

// Linear interpolation
vec3f mid = lerp(a, b, 0.5f);           // {5, 5, 5}

// Smoothstep
vec3f smooth = smoothstep(a, b, t);

// Bounce
vec3f bounce = bounce(a, b, t);

// Elastic
vec3f elastic = elastic(a, b, t);
```

## Serialization

```cpp
#include <xcmath/serialize.hpp>

using namespace xcmath;

vec3f v{1.5f, 2.5f, 3.5f};
std::cout << v << std::endl;  // Output: [1.5, 2.5, 3.5]

mat3f m = mat3f::unit();
std::cout << m << std::endl;
```

## Build

```bash
# Configure
cmake -B build

# Build
cmake --build build

# Run tests (requires Google Test)
cmake -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build
```

## Requirements

- C++20 compatible compiler (MSVC, Clang, GCC)
- CMake 3.15+
- [xcmixin](https://github.com/xcrtp/xcmixin) (included as submodule)

## Compatibility

| Compiler | Status |
|----------|--------|
| clang/clang-cl | Full support |
| gcc | Full support |
| MSVC | Full support |

## Project Structure

```
xcmath/
  xcmath.hpp           # Main include (optional)
  vec.hpp              # vec<T, N> template
  mat.hpp              # mat<T, R, C> template
  methods.hpp          # CRTP method framework
  alias.hpp            # Type aliases
  functions.hpp        # Free functions
  traits.hpp           # Type traits
  number_meta.hpp      # Numeric type properties
  point_accesser.hpp   # x(), y(), z(), w() accessors
  transform_methods.hpp # Transformation matrices
  interpolation.hpp    # Interpolation functions
  serialize.hpp        # Stream output support
```
