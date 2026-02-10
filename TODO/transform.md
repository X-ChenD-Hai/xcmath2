# 计算机图形学变换矩阵

## 模型变换

- [x] `translate(mat, vec3)` - 平移矩阵
- [x] `rotate(mat, angle, axis)` - 旋转矩阵 (绕任意轴)
- [x] `rotate_x(angle)` / `rotate_y(angle)` / `rotate_z(angle)` - 绕坐标轴旋转
- [x] `scale(mat, vec3)` - 缩放矩阵
- [x] `shear(mat, factors)` - 剪切矩阵

## 视图变换

- [x] `look_at(eye, center, up)` - 观察矩阵

## 投影变换

- [x] `perspective(fov, aspect, near, far)` - 透视投影矩阵
- [x] `ortho(left, right, bottom, top, near, far)` - 正交投影矩阵
- [x] `frustum(left, right, bottom, top, near, far)` - 视锥体投影
