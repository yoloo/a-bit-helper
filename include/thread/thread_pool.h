#ifndef __THREAD_H__
#define __THREAD_H__

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <stdexcept>
#include <memory>

class ThreadPool {
public:
    template<class Callback, class... Args>
    auto enqueue(Callback&& cb, Args&&... args) -> std::future<typename std::result_of<Callback(Args...)>::type> {
        using ReturnType = typename std::result_of<Callback(Args...)>::type;

        // #Note#
        // std::packaged_task include two objects:
        // -> stored task->lambda or functor;
        // -> shared state->to save return value;
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<Callback>(cb), std::forward<Args>(args)...)
                );

        // #Note#
        // future object bind with task's shared state.
        std::future<ReturnType> future_result = task->get_future();

        // Into queue and notify threads.
        {
            std::unique_lock<std::mutex> lock(this->mutex_);

            this->work_tasks_.emplace([task]() {
                (*task)();
            });
        }
        this->condition_.notify_all();

        return future_result;
    }

public:
    explicit ThreadPool(size_t threads);
    virtual ~ThreadPool();

public:
    void final();

private:
    std::vector<std::thread> work_threads_;

    std::queue<std::function<void()>> work_tasks_;

    std::mutex mutex_;
    std::condition_variable condition_;
    std::atomic_bool pool_stop_;
};


#endif//__THREAD_H__