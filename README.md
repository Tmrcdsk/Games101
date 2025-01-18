# Assignment 1 旋转与投影
## 环境
wsl2
## 编译
```bash
mkdir build
cd build
cmake ..
make -j4
```
## 运行
### 使用 A键 和 D键 旋转
```bash
./Rasterizer 
```
### 绕 z 轴旋转 20°，结果保存至 image.png
```bash
./Rasterizer -r 20 image.png
```
### 绕轴(0, 0, -1)旋转 20°，结果保存至 image.png
```bash
./Rasterizer -r 20 image.png 0 0 -1
```