//
// Created by sonu gupta
//

#ifndef THREADPOOL_THREADPOOL_H
#define THREADPOOL_THREADPOOL_H

#include <iostream>
#include <thread>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
using namespace std;

class threadpool
{
private:
    bool isQuit;
    std::mutex m;
    int thread_cnt;
    std::vector<std::thread> pool;
    std::condition_variable cv;
    std::queue<std::function<void()>> dispatch_queue;

public:
    threadpool(int count):thread_cnt{count}, isQuit{false}
    {
        initialize_pool(count);
    }

    void initialize_pool(int count);
    void add_to_queue(std::function<void()> f);
    void dispatch_handler();
    ~threadpool();
};

#endif //THREADPOOL_THREADPOOL_H
