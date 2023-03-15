//
// Created by zhaoshuhao on 2023/3/15.
//

#ifndef SYNC_ASYNC_THREAD_POOL_THREADPOOL_H
#define SYNC_ASYNC_THREAD_POOL_THREADPOOL_H

#include "sync_queue.h"
#include <atomic>
#include <functional>
#include <memory>

const int MaxTaskCount = 100;

class ThreadPool {
public:
    using task = std::function<void()>;
    ThreadPool(size_t thread_num = std::thread::hardware_concurrency(),size_t task_count = MaxTaskCount) : que_(task_count) {
        start(thread_num);
    }

    ~ThreadPool() { stop(); }
    void stop() {
        std::call_once(flag_, [this] { stop_group(); });
    }

    void add_task(task &&t) {
        que_.put(std::forward<task>(t));
    }

private:
    void start(size_t thread_num) {
        running_ = true;
        for (size_t i = 0; i < thread_num; i++) {
            thread_group_.push_back(std::make_shared<std::thread>(&ThreadPool::run_thread, this));
        }
    }

    void run_thread() {
        while (running_) {
            std::list<task> list;
            que_.take(list);
            for (auto &l: list) {
                if (!running_) return;
                l();
            }
        }
    }

    void stop_group() {
        running_ = false;
        que_.stop();

        for (auto &thread: thread_group_) {
            if (thread) thread->join();
        }
        thread_group_.clear();
    }

    std::list<std::shared_ptr<std::thread>> thread_group_;
    SyncQueue<task> que_;
    std::atomic_bool running_;
    std::once_flag flag_;
};
#endif//SYNC_ASYNC_THREAD_POOL_THREADPOOL_H
