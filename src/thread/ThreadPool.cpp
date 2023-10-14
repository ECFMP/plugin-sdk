#include "ThreadPool.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace ECFMP::Thread {
    struct ThreadPool::Impl {
        std::vector<std::thread> threads;

        std::mutex mutex;

        std::condition_variable condition;

        std::queue<std::function<void()>> tasks;

        bool running = true;
    };

    ThreadPool::ThreadPool() : impl(std::make_unique<ThreadPool::Impl>())
    {
        // Create 2 threads in the pool
        for (int i = 0; i < 2; i++) {
            impl->threads.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;

                    // Run this block in a lock
                    {
                        std::unique_lock<std::mutex> lock(impl->mutex);

                        // Wait for a task to be available, or for the pool to be stopped
                        impl->condition.wait(lock, [this]() {
                            return !impl->running || !impl->tasks.empty();
                        });

                        // If the pool is stopped, exit
                        if (!impl->running) {
                            return;
                        }

                        // If the pool doesn't have a task, continue
                        if (impl->tasks.empty()) {
                            continue;
                        }

                        // Grab the next task
                        task = std::move(impl->tasks.front());
                        impl->tasks.pop();
                    }

                    // Run the task
                    task();
                }
            });
        }
    }

    ThreadPool::~ThreadPool()
    {
        // Join all threads
        {
            std::unique_lock<std::mutex> lock(impl->mutex);
            impl->running = false;
        }

        impl->condition.notify_all();
        for (auto& thread: impl->threads) {
            thread.join();
        }
    }

    void ThreadPool::Schedule(const std::function<void()>& function)
    {
        // Run this block in a lock, add the task to the queue
        {
            std::unique_lock<std::mutex> lock(impl->mutex);
            impl->tasks.emplace(function);
        }

        // Notify a thread that a task is available
        impl->condition.notify_one();
    }
}// namespace ECFMP::Thread
