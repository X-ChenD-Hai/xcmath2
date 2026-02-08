# xcmath API 开发待办总览

## 模块索引

| 模块         | 文件                                   | 状态   |
| ------------ | -------------------------------------- | ------ |
| 线性代数基础 | [linear-algebra.md](linear-algebra.md) | 已完成 |
| 矩阵操作     | [matrix.md](matrix.md)                 | 已完成 |
| 变换矩阵     | [transform.md](transform.md)           | 待开始 |
| 插值函数     | [interpolation.md](interpolation.md)   | 待开始 |
| 四元数       | [quaternion.md](quaternion.md)         | 待开始 |
| 颜色空间     | [color.md](color.md)                   | 待开始 |
| 几何工具     | [geometry.md](geometry.md)             | 待开始 |
| 噪声函数     | [noise.md](noise.md)                   | 待开始 |
| 常用常量     | [constants.md](constants.md)           | 待开始 |

## 已完成功能

- [x] `size()` - 获取向量维度
- [x] `module()` - 计算向量模长
- [x] `normalize()` - 向量归一化
- [x] `zero()` / `unit()` - 工厂方法
- [x] `point_accesser` - 点访问器 (x, y, z, w)
