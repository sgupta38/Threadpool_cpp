#include "threadpool.h"

void threadpool::initialize_pool(int count)
{
    for(int i= 0; i< thread_cnt; ++i)
        pool.emplace_back(&threadpool::dispatch_handler, this);
}

void threadpool::add_to_queue(const fp_t& f)
{
    std::unique_lock<std::mutex> lock_(m);
    dispatch_queue.push(f);
    lock_.unlock(); // unlock will be done anyway by end of function. But reducing latency.
    cv.notify_all();
}

void threadpool::add_to_queue(fp_t&& f)
{
    std::unique_lock<std::mutex> lock_(m);
    dispatch_queue.push(std::move(f));
    lock_.unlock(); // unlock will be done anyway by end of function. But reducing latency.
    cv.notify_all();
}

void threadpool::dispatch_handler()
{
    std::unique_lock<std::mutex> lock_(m);
    do{
        cv.wait(lock_, [this] { return (dispatch_queue.size() || isQuit); });
        if(!isQuit && dispatch_queue.size())
        {
            auto task = dispatch_queue.front();
            dispatch_queue.pop();
            lock_.unlock(); // unlock since we are done using shared resource (queue)

            task(); // execute function

            lock_.lock();
        }
    }while(!isQuit);
}

threadpool::~threadpool()
{
    std::unique_lock<std::mutex> lock_(m);
    isQuit = true;
    lock_.unlock();
    cv.notify_all();

    for(int i = 0; i < pool.size(); ++i)
    {
        if(pool[i].joinable())
            pool[i].join();
    }
}