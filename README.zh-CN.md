# xcmath - C++ 线性代数库

[![GitHub License](https://img.shields.io/github/license/xcrtp/xcmath)](https://github.com/xcrtp/xcmath/blob/main/LICENSE)
[![GitHub Release](https://img.shields.io/github/v/release/xcrtp/xcmath)](https://github.com/xcrtp/xcmath/releases)
[![GitHub Tag](https://img.shields.io/github/v/tag/xcrtp/xcmath)](https://github.com/xcrtp/xcmath/tags)
![GitHub top language](https://img.shields.io/github/languages/top/xcrtp/xcmath?style=flat)

## 项目概述

**xcmath** 是一个现代化的 C++20 头文件式线性代数库，提供 `vec<T, N>` 和 `mat<T, R, C>` 类型，采用基于 CRTP 的方法组合框架。为 xcal 项目设计。

## 核心特性

- **头文件式**: 无需编译，直接包含使用
- **CRTP 方法**: 编译时方法组合，无虚函数开销
- **类型安全**: 完整的编译时类型检查
- **灵活性**: 支持自定义数值类型和视图类型切片

## 快速示例

```cpp
#include <xcmath/vec.hpp>
#include <xcmath/mat.hpp>

using namespace xcmath;

// 向量运算
vec3f v1{1, 2, 3};
vec3f v2{4, 5, 6};

float dotProduct = v1.dot(v2);        // 点积
vec3f crossProduct = v1.cross(v2);   // 叉积
float magnitude = v1.module();       // 长度
vec3f normalized = v1.normalize();  // 归一化向量

// 矩阵运算
mat4f m = mat4f::unit();             // 单位矩阵
m = m.rotate(45.0f, vec3f{0, 0, 1}) // 旋转
     .scale(2.0f)                    // 缩放
     .translate(vec3f{1, 2, 3});    // 平移

// 矩阵向量乘法
vec4f result = m * vec4f{1, 0, 0, 1};
```

## 向量类型

### 基本向量

```cpp
#include <xcmath/vec.hpp>

using namespace xcmath;

// 类型别名（默认为 float）
vec2f v2;    // 2D 向量
vec3f v3;    // 3D 向量
vec4f v4;    // 4D 向量

// 自定义大小和类型
vec<double, 10> v10;    // 10维 double 向量
vec<int, 3> vi3;        // 3维 int 向量

// 初始化
vec3f v1{1, 2, 3};              // 花括号初始化
vec3f v2 = vec3f::zero();        // 零向量
vec3f v3 = vec3f::unit();       // 单位向量 (1, 1, 1)
```

### 点访问器

对于大小为 1-4 的向量，可以使用点访问器：

```cpp
vec2f v2{1, 2};
float x = v2.x();    // 1
float y = v2.y();    // 2

vec3f v3{1, 2, 3};
float z = v3.z();    // 3

vec4f v4{1, 2, 3, 4};
float w = v4.w();    // 4
```

### 向量方法

| 方法 | 描述 | 示例 |
|------|------|------|
| `size()` | 返回向量维度 | `v.size()` → 3 |
| `module()` | 返回向量长度 | `v.module()` → 5.0 |
| `normalize()` | 返回单位向量 | `v.normalize()` |
| `dot(v)` | 点积 | `v1.dot(v2)` |
| `cross(v)` | 叉积（仅 vec3） | `v1.cross(v2)` |
| `distance(v)` | 到另一向量的距离 | `v1.distance(v2)` |
| `distance_squared(v)` | 平方距离 | `v1.distance_squared(v2)` |
| `angle(v)` | 向量间角度（弧度） | `v1.angle(v2)` |
| `project(v)` | 投影到向量 | `v1.project(v2)` |
| `reflect(normal)` | 绕法线反射 | `v.reflect(n)` |

### 分量运算

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
a.min(b);       // {1, 2, 3}  - 分量最小值
a.max(b);       // {3, 5, 4}  - 分量最大值
a.clamp(lo, hi);// 分量限制
```

### 比较运算

```cpp
vec3f v1{1, 2, 3};
vec3f v2{1, 2, 4};

v1 == v2;       // 分量相等
v1 != v2;       // 分量不等

v1.equal(v2, epsilon);   // 带容差比较
v1.less_than(v2);        // 分量比较（返回 vec）
v1.greater_than(v2);     // 分量比较（返回 vec）
v1.any();                // 任意分量为非零
v1.all();                // 所有分量为非零
```

## 矩阵类型

### 基本矩阵

```cpp
#include <xcmath/mat.hpp>

using namespace xcmath;

// 类型别名（默认行主序）
mat2f m2;    // 2x2 矩阵
mat3f m3;    // 3x3 矩阵
mat4f m4;    // 4x4 矩阵

// 自定义大小和类型
mat<double, 3, 4> m;    // 3x4 矩阵

// 初始化
mat3f m1{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};  // 行主序
mat3f m2 = mat3f::unit();                    // 单位矩阵
mat3f m3 = mat3f::zero();                    // 零矩阵
```

### 列主序矩阵

在包含头文件前定义 `ENABLE_COL_MAJOR_MAT`：

```cpp
#define ENABLE_COL_MAJOR_MAT
#include <xcmath/mat.hpp>

using namespace xcmath;
mat4f colMajor = mat4f::unit();  // 现在是列主序
```

### 矩阵方法

| 方法 | 描述 |
|------|------|
| `transpose()` | 返回转置矩阵 |
| `determinant()` | 返回行列式（2x2, 3x3, 4x4） |
| `inverse()` | 返回逆矩阵 |
| `trace()` | 返回对角元素之和 |
| `rotate(angle, axis)` | 返回旋转矩阵 |
| `rotate_x/y/z(angle)` | 返回绕轴旋转矩阵 |
| `scale(s)` | 返回缩放矩阵 |
| `translate(v)` | 返回平移矩阵 |
| `look_at(eye, center, up)` | 返回视图矩阵 |

### 矩阵运算

```cpp
mat3f m1{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
mat3f m2 = mat3f::unit();

mat3f sum = m1 + m2;                     // 矩阵加法
mat3f diff = m1 - m2;                    // 矩阵减法
mat3f prod = m1 * m2;                    // 矩阵乘法
vec3f v = m1 * vec3f{1, 2, 3};          // 矩阵向量乘法

// 自由函数
mat3f t = transpose(m1);                 // 转置
float d = determinant(m1);               // 行列式
mat3f i = inverse(m1);                   // 逆矩阵
float tr = trace(m1);                     // 迹
mat3f outer = outer_product(v1, v2);     // 外积
```

## 变换

```cpp
#include <xcmath/transform_methods.hpp>

mat4f transform = mat4f::unit()
    .rotate(45.0f, vec3f{0, 0, 1})   // 绕 Z 轴旋转 45°
    .scale(2.0f)                      // 2倍缩放
    .translate(vec3f{1, 2, 3});       // 平移

vec4f result = transform * vec4f{1, 0, 0, 1};
```

## 插值

```cpp
#include <xcmath/interpolation.hpp>

using namespace xcmath;

vec3f a{0, 0, 0};
vec3f b{10, 10, 10};

// 线性插值
vec3f mid = lerp(a, b, 0.5f);           // {5, 5, 5}

// 平滑步进
vec3f smooth = smoothstep(a, b, t);

// 弹跳
vec3f bounce = bounce(a, b, t);

// 弹性
vec3f elastic = elastic(a, b, t);
```

## 序列化

```cpp
#include <xcmath/serialize.hpp>

using namespace xcmath;

vec3f v{1.5f, 2.5f, 3.5f};
std::cout << v << std::endl;  // 输出: [1.5, 2.5, 3.5]

mat3f m = mat3f::unit();
std::cout << m << std::endl;
```

## 构建

```bash
# 配置
cmake -B build

# 构建
cmake --build build

# 运行测试（需要 Google Test）
cmake -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build
```

## 环境要求

- C++20 兼容编译器（MSVC、Clang、 GCC）
- CMake 3.15+
- [xcmixin](https://github.com/xcrtp/xcmixin)（作为子模块包含）

## 兼容性

| 编译器 | 状态 |
|--------|------|
| clang/clang-cl | 完全支持 |
| gcc | 完全支持 |
| MSVC | 完全支持 |

## 项目结构

```
xcmath/
  xcmath.hpp           # 主包含文件（可选）
  vec.hpp              # vec<T, N> 模板
  mat.hpp              # mat<T, R, C> 模板
  methods.hpp          # CRTP 方法框架
  alias.hpp            # 类型别名
  functions.hpp        # 自由函数
  traits.hpp           # 类型特征
  number_meta.hpp      # 数值类型属性
  point_accesser.hpp   # x(), y(), z(), w() 访问器
  transform_methods.hpp # 变换矩阵
  interpolation.hpp    # 插值函数
  serialize.hpp        # 流输出支持
```
