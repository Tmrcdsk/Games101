# Assignment 7 路径追踪
## 编译
```bash
mkdir build
cd build
cmake ..
make
```
## 运行
```bash
./RayTracing
```
## 运行结果
### SPP = 4
![SPP=4](images/binary4.png)
![time](images/SPP4.png)

### SPP = 16
![SPP=16](images/binary16.png)
![time](images/SPP16.png)

### SPP = 4（多线程）
![SPP=4](images/Threadbinary4.png)
![time](images/ThreadSPP4.png)

### SPP = 16（多线程）
![SPP=16](images/Threadbinary16.png)
![time](images/ThreadSPP16.png)

## 学习笔记
- 在 [Bounds3.hpp](Bounds3.hpp) 中，需要将判断条件 `if (t_exit > 0.0 && t_enter < t_exit)` 改为 `if (t_exit > 0.0 && t_enter <= t_exit)`，这里一定要判断相等的情况，不然渲染结果会出错！
- 在 [global.hpp](global.hpp) 中，将 `get_random_float()` 函数改为 `static`，能够加速随机数的生成
- 按以下伪代码实现路径追踪
![PseudoCode](images/PseudoCode.png)
- 路径追踪图示</br>
![PathTracing](images/PathTracing.png)
- 使用 `std::thread` 和 `std::mutex` 来实现多线程（[C++11 多线程（std::thread）详解](https://blog.csdn.net/sjc_0910/article/details/118861539)），自己写的多线程例子 [examples](Ref/Multi-thread.md)