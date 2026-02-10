# 计算机图形学插值

## 基础插值

- [X] `lerp(a, b, t)` - 线性插值
- [X] `mix(a, b, t)` - 混合 (lerp别名)
- [X] `step(edge, x)` - 阶跃函数
- [X] `smoothstep(edge0, edge1, x)` - 平滑阶跃
- [X] `smootherstep(edge0, edge1, x)` - 更平滑的阶跃 (Perlin改进版)

## 高级插值

- [X] `bezier(p0, p1, p2, t)` - 二次贝塞尔曲线
- [X] `bezier(p0, p1, p2, p3, t)` - 三次贝塞尔曲线
- [X] `catmull_rom(p0, p1, p2, p3, t)` - Catmull-Rom 样条
- [X] `hermite(p0, m0, p1, m1, t)` - Hermite 插值

## 球面插值

- [X] `slerp(q1, q2, t)` - 球面线性插值 (四元数)
- [X] `nlerp(q1, q2, t)` - 归一化线性插值
