
#ifndef ANH_UTILS_ACTIVE_OBJECT_H_
#define ANH_UTILS_ACTIVE_OBJECT_H_

#include <algorithm>
#include <list>
#include <memory>

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

#include <boost/thread/future.hpp>

#include "anh/utils/timing.h"

namespace anh {
namespace utils {

/** 
 * There are many times when it makes sense to break an object off and run it
 * concurrently while the rest of the application runs. The ActiveObject is a
 * reusable facility that encourages the encapsulation of data by using asynchronus
 * messages to process requests in a private thread. 
 * 
 * This implementation is based on boost::asio and uses an io_service coupled with a
 * strand to facilitate the sequential requirements of an active objects message handling.
 */
class ActiveObject {
public:
    explicit ActiveObject(boost::asio::io_service& io_service)
        : io_service_(io_service)
        , strand_(io_service) {}
    
    /**
     * Sends a message to be handled by the ActiveObject's asio strand. Delays
     * execution of the message by a specified period of time.
     *
     * @param period The period of time to delay execution of the given message.
     * @param message The message to be handled by the active object.
     */
    template<typename Handler>
    void SendDelayed(boost::posix_time::time_duration period, Handler message) {
        auto timer = std::make_shared<boost::asio::deadline_timer>(io_service_);
        
        timer->expires_from_now(period);
        timer->async_wait(strand_.wrap([timer, message] (const boost::system::error_code& error) {
            message(error);   
        }));
    }
    
    /**
     * Repeatedly sends a message to be handled by the ActiveObject's asio 
     * strand. The specified period of time determines the interval between resends.
     *
     * @param period The period of time to delay execution of the given message.
     * @param message The message to be handled by the active object.
     */
    template<typename Handler>
    void SendRepeated(boost::posix_time::time_duration period, Handler message) {
        auto timer = std::make_shared<boost::asio::deadline_timer>(io_service_);
        
        AsyncRepeat(timer, period, strand_.wrap(message));
    }


    /**
     * Triggers an asyncronous task on the active object's strand. Returns a future
     * containing the return value. Can be used to confirm when the async task has completed.
     *
     * @param func The function handler to perform and retrieve results of asyncronously.
     */
    template<typename T>
    boost::unique_future<typename std::result_of<T()>::type>
    Async(T func) {
        auto task = std::make_shared<boost::packaged_task<typename std::result_of<T()>::type>>(func);
        
        strand_.post([task] () {
            (*task)();
        });

        return task->get_future();
    }

private:
    ActiveObject();

    boost::asio::io_service& io_service_;
    boost::asio::strand strand_;
};
    
}}  // namespace anh::utils

#endif  // ANH_UTILS_ACTIVE_OBJECT_H_
