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
    typedef std::function<void(void)> fp_t;
private:
    bool isQuit;
    std::mutex m;
    int thread_cnt;
    std::vector<std::thread> pool;
    std::condition_variable cv;
    std::queue<fp_t> dispatch_queue;

public:

    threadpool(int count):thread_cnt{count}, isQuit{false}
    {
        initialize_pool(count);
    }

    void initialize_pool(int count);
    void add_to_queue(const fp_t& f); // dispatch and copy
    void add_to_queue(fp_t&& f);      // dispatch and move
    void dispatch_handler();
    ~threadpool();

    //deleted functions
    threadpool(const threadpool& rhs) = delete;
    threadpool(threadpool&& rhs) = delete;
    threadpool& operator=(const threadpool& rhs) = delete;
    threadpool& operator=(threadpool&& rhs) = delete;
};

#endif //THREADPOOL_THREADPOOL_H
