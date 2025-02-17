## 创建线程
```cpp
#include <iostream>
#include <thread>

void printMessage(int count)
{
    for (int i = 0; i < count; ++i) {
        std::cout << "Hello from thread (function pointer)!\n";
    }
}

class PrintTask
{
public:
    void operator()(int count) const {
        for (int i = 0; i < count; ++i) {
            std::cout << "Hello from thread (function object)!\n";
        }
    }
};

int main()
{
    // 获取当前设备建议的线程数 = CPU核心数
    std::cout << std::thread::hardware_concurrency() << std::endl;

    std::thread t1(printMessage, 5);    // 1. 函数指针
    std::thread t2(PrintTask(), 5);     // 2. 函数对象
    std::thread t3([](int count) {      // 3. lambda函数
        for (int i = 0; i < count; ++i) {
            std::cout << "Hello from thread (lambda)!\n";
        }
    }, 5);
    t1.join(); // 等待当前线程完成
    t2.join();
    t3.join();
    return 0;
}
```

## 互斥防止冲突操作
```cpp
#include <mutex>
#include <thread>
#include <iostream>

std::mutex mtx; // 全局互斥量

int num = 2;

void safeFunction() {
    mtx.lock(); // 请求锁定互斥量
    // 访问或修改共享资源
    for (int i = 0; i < 10; ++i)
        std::cout << (num * i) << std::endl;
    mtx.unlock(); // 释放互斥量
}

int main() {
    std::thread t1(safeFunction);
    std::thread t2(safeFunction);
    t1.join();
    t2.join();
    return 0;
}
```

## 多线程优化for循环
```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cmath>

const int N = 5000000;

class Timer
{
public:
    Timer() : start(std::chrono::system_clock::now())
    {
        // 构造函数：记录开始时间
    }

    ~Timer()
    {
        // 析构函数：计算并打印经过的时间
        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Elapsed time: " << duration.count() << " milliseconds" << std::endl;
    }

private:
    std::chrono::system_clock::time_point start;
};

int main()
{
    std::vector<int> data(N, 2);
    {
        Timer timer;
        for (int i = 0; i < N; ++i)
        {
            data[i] = std::pow(data[i], 2);
        }
        std::cout << "Single-threaded loop finished." << std::endl;
    }
    std::cout << "data[0]: " << data[0] << std::endl;

    std::fill(data.begin(), data.end(), 2);

    const int num_threads = std::thread::hardware_concurrency();
    std::cout << "Using " << num_threads << " threads." << std::endl;
    {
        std::vector<std::thread> threads;
        threads.reserve(num_threads);

        Timer timer;
        for (int t = 0; t < num_threads; ++t)
        {
            int start = t * N / (num_threads);
            int end = (t == num_threads) ? N : ((t + 1) * N / (num_threads));
            threads.emplace_back([start, end, &data]()
                                 {
                for (int i = start; i < end; ++i)
                {
                    data[i] = std::pow(data[i], 2);
                } });
        }
        for (auto &thread : threads)
            thread.join();
        std::cout << "Multi-threaded loop finished." << std::endl;
    }
    std::cout << "data[0]: " << data[0] << std::endl;

    return 0;
}
```