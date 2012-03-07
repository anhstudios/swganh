
#ifndef ANH_DELAYED_TASK_PROCESSOR_H_
#define ANH_DELAYED_TASK_PROCESSOR_H_

#include <memory>
#include <queue>

#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/thread/mutex.hpp>

namespace anh {

    template<typename Task = std::function<void ()>>
    class DelayedTaskProcessor
    {        
        struct TaskInfo
        {
            TaskInfo(boost::posix_time::time_duration delay, Task&& task)
                : delay(delay), task(move(task))
            {}
                
            boost::posix_time::time_duration delay;
            Task task;
        };

    public:
        DelayedTaskProcessor(boost::asio::io_service& io_service)
            : timer_(io_service)
            , processing_(false)
        {}
            
        void PushTask(boost::posix_time::time_duration delay, Task&& task)
        {
            {
                boost::lock_guard<boost::mutex> lg(queue_mutex_);
                queue_.push(std::make_shared<TaskInfo>(delay, move(task)));
            }
            
            Notify();
        }
        
    private:
        DelayedTaskProcessor();
        
        void Notify()
        {
            boost::lock_guard<boost::mutex> lg(process_mutex_);
            if (!processing_)
            {
                boost::lock_guard<boost::mutex> lg(queue_mutex_);
                if (!queue_.empty())
                {
                    auto& task_info = queue_.front();
                    
                    timer_.expires_from_now(task_info->delay);
                    timer_.async_wait([this, task_info] (const boost::system::error_code& ec) {
                        if (!ec)
                        {
                            task_info->task();
                        }
                        
                        {
                            boost::lock_guard<boost::mutex> lg(process_mutex_);
                            processing_ = false;
                        }

                        this->Notify();
                    });

                    queue_.pop();
                    
                    processing_ = true;				
                }
            }		
        }
        
        boost::asio::deadline_timer	timer_;
        
        boost::mutex process_mutex_;
        bool processing_;
        
        boost::mutex queue_mutex_;	
        std::queue<std::shared_ptr<TaskInfo>> queue_;
    };
    
    typedef DelayedTaskProcessor<std::function<void ()>> SimpleDelayedTaskProcessor;

}  // namespace anh

#endif  // ANH_DELAYED_TASK_PROCESSOR_H_

