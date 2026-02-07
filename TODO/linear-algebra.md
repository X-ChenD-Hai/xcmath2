# 线性代数基础操作

## 向量运算

- [x] `dot(v1, v2)` - 点积/内积
- [x] `cross(v1, v2)` - 叉积 (vec3)
- [x] `distance(v1, v2)` - 两点间距离
- [x] `distance_squared(v1, v2)` - 两点间距离的平方 (避免开方)
- [x] `angle(v1, v2)` - 两向量夹角
- [x] `project(v, onto)` - 向量投影
- [x] `reflect(v, normal)` - 向量反射
- [x] `refract(v, normal, eta)` - 向量折射 (Snell定律)

## 向量分量操作

- [x] `abs(v)` - 各分量取绝对值
- [x] `min(v1, v2)` - 各分量取最小值
- [x] `max(v1, v2)` - 各分量取最大值
- [x] `clamp(v, min, max)` - 各分量限制范围
- [x] `floor(v)` - 各分量向下取整
- [x] `ceil(v)` - 各分量向上取整
- [x] `round(v)` - 各分量四舍五入
- [x] `fract(v)` - 各分量取小数部分
- [x] `sign(v)` - 各分量取符号 (-1, 0, 1)

## 向量比较

- [x] `equal(v1, v2, epsilon)` - 近似相等判断
- [x] `less_than(v1, v2)` - 分量比较 (<)
- [x] `greater_than(v1, v2)` - 分量比较 (>)
- [x] `any(bvec)` - 任一分量为真
- [x] `all(bvec)` - 所有分量为真
