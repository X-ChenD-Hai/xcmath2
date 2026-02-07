# 线性代数基础操作

## 向量运算

- [ ] `dot(v1, v2)` - 点积/内积
- [ ] `cross(v1, v2)` - 叉积 (vec3)
- [ ] `distance(v1, v2)` - 两点间距离
- [ ] `distance_squared(v1, v2)` - 两点间距离的平方 (避免开方)
- [ ] `angle(v1, v2)` - 两向量夹角
- [ ] `project(v, onto)` - 向量投影
- [ ] `reflect(v, normal)` - 向量反射
- [ ] `refract(v, normal, eta)` - 向量折射 (Snell定律)

## 向量分量操作

- [ ] `abs(v)` - 各分量取绝对值
- [ ] `min(v1, v2)` - 各分量取最小值
- [ ] `max(v1, v2)` - 各分量取最大值
- [ ] `clamp(v, min, max)` - 各分量限制范围
- [ ] `floor(v)` - 各分量向下取整
- [ ] `ceil(v)` - 各分量向上取整
- [ ] `round(v)` - 各分量四舍五入
- [ ] `fract(v)` - 各分量取小数部分
- [ ] `sign(v)` - 各分量取符号 (-1, 0, 1)

## 向量比较

- [ ] `equal(v1, v2, epsilon)` - 近似相等判断
- [ ] `less_than(v1, v2)` - 分量比较 (<)
- [ ] `greater_than(v1, v2)` - 分量比较 (>)
- [ ] `any(bvec)` - 任一分量为真
- [ ] `all(bvec)` - 所有分量为真
