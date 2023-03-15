//
// Created by zhaoshuhao on 2023/3/15.
//

#ifndef SYNC_ASYNC_THREAD_POOL_SYNCQUEUE_H
#define SYNC_ASYNC_THREAD_POOL_SYNCQUEUE_H
#include <condition_variable>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>

template<typename T>
class SyncQueue {
public:
    explicit SyncQueue(int max_size) : max_size_(max_size) {}
    ~SyncQueue() { stop_ = true; }

//    void put(const T &t) {
//        add(t);
//    }

    void put(T &&t) {
        add(t);
    }

    //stop()
    void stop() {
        {
            std::lock_guard<std::mutex> lockGuard(mutex_);
            stop_ = true;
        }
        conditionVariable.notify_all();
    }

    //take()
    void take(std::list<T> &list) {
        std::unique_lock<std::mutex> uniqueLock(mutex_);
        conditionVariable.wait(uniqueLock, [this] {
            return stop_ || not_empty();
        });

        if (stop_) return;
        list = std::move(queue_);
        conditionVariable.notify_one();
    }

    void take(T &t) {
        std::unique_lock<std::mutex> uniqueLock(mutex_);
        conditionVariable.wait(uniqueLock, [this] {
            return stop_ || not_empty();
        });

        if (stop_) return;
        t = queue_.front();
        queue_.pop_front();
        conditionVariable.notify_one();
    }

    //empty()
    bool empty() {
        std::lock_guard<std::mutex> lockGuard(mutex_);
        return queue_.empty();
    }

    //full()
    bool full() {
        std::lock_guard<std::mutex> lockGuard(mutex_);
        return queue_.size() >= max_size_;
    }

    //size()
    size_t size() {
        std::lock_guard<std::mutex> lockGuard(mutex_);
        return queue_.size();
    }

    //count();
    bool count() {
        return size();
    }

private:
    bool not_empty() {
        if (!queue_.empty()) return true;
        std::cout << "queue is empty  Async thread id = " << std::this_thread::get_id() << std::endl;
        return false;
    }

    bool not_full() {
        bool full = queue_.size() >= max_size_;
        if (!full) return true;
        std::cout << "queue is full Sync thread id = " << std::this_thread::get_id() << std::endl;
        return false;
    }

    template<typename F>
    void add(F &&f) {
        std::unique_lock<std::mutex> lock(mutex_);
        conditionVariable.wait(lock, [this] {
            return stop_ || not_full();
        });

        if (stop_) return;
        queue_.push_back(std::forward<F>(f));
        conditionVariable.notify_one();
    }

private:
    std::list<T> queue_;
    std::condition_variable conditionVariable;
    std::mutex mutex_;

    bool stop_ = false;
    int max_size_;
};
#endif//SYNC_ASYNC_THREAD_POOL_SYNCQUEUE_H
