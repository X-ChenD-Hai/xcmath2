# 四元数 (Quaternion)

## 类型定义

- [ ] `quat` 类型定义

## 创建方法

- [ ] `quat_from_axis_angle(axis, angle)` - 从轴角创建
- [ ] `quat_from_euler(pitch, yaw, roll)` - 从欧拉角创建
- [ ] `quat_from_mat3(mat)` / `quat_from_mat4(mat)` - 从旋转矩阵创建

## 转换方法

- [ ] `quat_to_mat3(q)` / `quat_to_mat4(q)` - 转换为旋转矩阵

## 基础操作

- [ ] `quat_conjugate(q)` - 共轭
- [ ] `quat_inverse(q)` - 求逆
- [ ] `quat_multiply(q1, q2)` - 四元数乘法
- [ ] `quat_rotate_vec(q, v)` - 用四元数旋转向量
