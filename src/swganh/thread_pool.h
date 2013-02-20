// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include <boost/noncopyable.hpp>

namespace swganh {

    class ThreadPool : private boost::noncopyable
    {
    public:
        ThreadPool(size_t thread_count);
        ~ThreadPool();

        template<typename T>
        std::future<typename std::result_of<T()>::type> Schedule(T&& task)
        {
            auto wrapped_task = std::make_shared<std::packaged_task<typename std::result_of<T()>::type()>>(std::move(task));

            auto result_future = wrapped_task->get_future();

            {
                std::unique_lock<std::mutex> lock(queue_mutex_);

                tasks_.emplace_back([wrapped_task] () {
                    (*wrapped_task)();
                });
            }

            condition_.notify_one();

            return result_future;
        }

    private:
        friend class Worker;

        ThreadPool();

        class Worker
        {
        public:
            Worker(ThreadPool& s) : pool_(s) {}

            void operator()();
        private:

            ThreadPool& pool_;
        };

        std::vector<std::thread> threads_;
        std::deque<std::function<void()>> tasks_;

        std::mutex queue_mutex_;
        std::condition_variable condition_;
        std::atomic<bool> stop_;
    };

}  // namespace swganh
