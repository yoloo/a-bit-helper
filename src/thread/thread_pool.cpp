#include "thread_pool.h"

ThreadPool::ThreadPool(size_t threads) : pool_stop_(false) {
    for (size_t i = 0; i < threads; ++i) {
        work_threads_.emplace_back([this]() {
            while (true) {
                std::function<void()> task;

                {
                    // Thread wait until tasks is empty or pool final.
                    std::unique_lock<std::mutex> lock(this->mutex_);

                    this->condition_.wait(lock, [this]() {
                        return !this->work_tasks_.empty() || this->pool_stop_;
                    });

                    // Pool final, still handle the left tasks.
                    if (this->pool_stop_ && this->work_tasks_.empty())
                        return;

                    // Get one task from queue.
                    task = std::move(this->work_tasks_.front());
                    this->work_tasks_.pop();
                }

                // Handle task.
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    this->final();
}

void ThreadPool::final() {
    // Set pool stop and notify all threads.
    {
        std::unique_lock<std::mutex> lock(this->mutex_);

        this->pool_stop_ = true;
    }
    this->condition_.notify_all();

    // Wait until work-thread done.
    for (auto& worker : work_threads_)
        worker.join();
}