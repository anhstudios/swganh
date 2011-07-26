
#ifndef ANH_UTILS_ACTIVE_OBJECT_H_
#define ANH_UTILS_ACTIVE_OBJECT_H_

#include <algorithm>
#include <list>
#include <memory>

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

#include "anh/utils/timing.h"

namespace anh {
namespace utils {

class ActiveObject {
public:
    explicit ActiveObject(boost::asio::io_service& io_service)
        : io_service_(io_service)
        , strand_(io_service) {}

    template<typename Handler>
    void Send(Handler message) {
        strand_.post(message);
    }

    template<typename Handler>
    void SendDelayed(boost::posix_time::time_duration period, Handler message) {
        auto timer = std::make_shared<boost::asio::deadline_timer>(io_service_);
        
        timer_->expires_from_now(period);
        timer_->async_wait(strand_.wrap(message));
    }

    
    template<typename Handler>
    void SendRepeated(boost::posix_time::time_duration period, Handler message) {
        auto timer = std::make_shared<boost::asio::deadline_timer>(io_service_);
        
        AsyncRepeat(timer, period, strand_.wrap(message));
    }

private:
    ActiveObject();

    boost::asio::io_service& io_service_;
    boost::asio::strand strand_;
};
    
}}  // namespace anh::utils

#endif  // ANH_UTILS_ACTIVE_OBJECT_H_
