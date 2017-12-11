#pragma once

#include <cstdlib>
#include <vector>
#include <queue>
#include <mutex>
#include <chrono>
#include <memory>
#include <thread>
#include <future>
#include <functional>
#include <condition_variable>
#include <exception>
#include <string>
#include <iostream>

class ThreadPool
{
public:
    explicit ThreadPool(size_t);
    ~ThreadPool();

    template<class Task, class... Args>
    auto addTaskToPool(Task&& task, Args&& ... args)
    -> std::future<typename std::result_of<Task(Args...)>::type>;
private:

    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> taskQueue;
    std::atomic<bool> disabled;
};

ThreadPool::ThreadPool(size_t workersCount) : disabled(false)
{
    if(workersCount < 2)
        throw std::runtime_error("Too few workers! Pointless");

    for (size_t i = 0; i < workersCount; i++)
    {
        workers.emplace_back([this]() {
            while (true)
            {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> locker(mtx);
                    cv.wait(locker, [this]() { return disabled == true || !taskQueue.empty(); });
                    if (taskQueue.empty() && disabled) return;
                    //Using move to obtain function from queue
                    task = std::move(taskQueue.front());
                    taskQueue.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> locker(mtx);
        disabled = true;
    }
    cv.notify_all();

    for(auto &thr : workers)
        thr.join();
}

template <class Func, class... Args>
auto ThreadPool::addTaskToPool(Func&& function, Args&& ... args)
-> std::future<typename std::result_of<Func(Args...)>::type>
{
    using returnType = typename std::result_of<Func(Args...)>::type;

    //Using make_shared to get copyable ptr on task, that can be executed async.
    auto task = std::make_shared<std::packaged_task<returnType()>>
            (std::bind(std::forward<Func>(function),std::forward<Args>(args)...));

    auto result = task->get_future();

    {
        std::unique_lock<std::mutex> locker(mtx);
        if (disabled) throw std::runtime_error("ThreadPool is stopped. Adding task is not available");
        taskQueue.emplace([task](){ (*task)();});
    }
    cv.notify_one();

    return result;
}