
#include "thread_pool.h"

using namespace swganh;

ThreadPool::ThreadPool(size_t thread_count)
    : stop_(false)
{
    for(size_t i = 0; i < thread_count; ++i)
    {
        threads_.emplace_back(Worker(*this));
    }
}

ThreadPool::~ThreadPool()
{
    stop_ = true;
    condition_.notify_all();

    for(auto& worker : threads_)
    {
        worker.join();
    }
}

void ThreadPool::Worker::operator()()
{
    std::function<void()> task;

    while(true)
    {
        {
            std::unique_lock<std::mutex> lock(pool_.queue_mutex_);

            while(!pool_.stop_ && pool_.tasks_.empty())
            {
                pool_.condition_.wait(lock);
            }

            if(pool_.stop_)
            {
                return;
            }

            task = pool_.tasks_.front();
            pool_.tasks_.pop_front();
        }

        task();
    }
}
