#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <map>
#include <mutex>
#include <map>
#include <atomic>
#include <functional>

#include "handler.h"

#define MIN_THREADS_COUNT 1

class ThreadPool {
private:
    std::vector<std::thread> threads;
    std::queue<std::function<int(int)>> tasks;
    std::mutex queueMutex;
    std::mutex mapMutex;
    std::atomic_bool stop;
    int maxEpollEvents;
    int timeout;

    HttpHandler* handler;

    void worker();
    void init(size_t threadCount);
    void pop(std::function<int(int)>* task);

public:
    ThreadPool(size_t threadCount, int maxEpollEvents, int timeout, HttpHandler* handler);
    ThreadPool();
    ~ThreadPool();

    void AddTask(std::function<int(int)> task);
};

#endif  // THREADPOOL_H
