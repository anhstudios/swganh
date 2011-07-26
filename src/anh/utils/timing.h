
#ifndef ANH_UTILS_TIMING_H_
#define ANH_UTILS_TIMING_H_

#include <boost/asio/deadline_timer.hpp>

namespace anh {
namespace utils {

template<typename Handler>
class RepeatHandler {
public:
    RepeatHandler(std::shared_ptr<boost::asio::deadline_timer> timer,
        boost::posix_time::time_duration period,
        Handler handler)
        : timer_(timer)
        , period_(period)
        , handler_(handler) {}

    void operator() (const boost::system::error_code& error) {
        if (error) {
            handler_(error);
        } else if (timer_->expires_at() == boost::posix_time::neg_infin) {
            boost::system::error_code new_error = boost::asio::error::operation_aborted;
            handler_(new_error);
        } else {
            handler_(error);
            timer_->expires_from_now(period_);
            timer_->async_wait(*this);
        }
    }

private:
    std::shared_ptr<boost::asio::deadline_timer> timer_;
    boost::posix_time::time_duration period_;
    Handler handler_;
};

template<typename Handler>
void AsyncRepeat(std::shared_ptr<boost::asio::deadline_timer> timer,
    boost::posix_time::time_duration period,
    Handler handler)
{
    timer->expires_from_now(period);
    timer->async_wait(RepeatHandler<Handler>(timer, period, handler));
}

}}  // namespace anh::utils

#endif  // ANH_UTILS_TIMING_H_
