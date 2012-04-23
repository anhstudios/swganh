// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_BASE_BASE_SERVICE_H_
#define SWGANH_BASE_BASE_SERVICE_H_

#include <atomic>
#include <memory>
#include <string>

#include <boost/asio.hpp>

#include "anh/service/service_interface.h"

#include "anh/active_object.h"

namespace anh {
namespace event_dispatcher {
class EventDispatcherInterface;
}}  // namespace anh::event_dispatcher

namespace swganh {
namespace app {
    class SwganhKernel;
}}  // namespace swganh::app

namespace swganh {
namespace base {

class BaseService : public anh::service::ServiceInterface {
public:    
    BaseService(swganh::app::SwganhKernel* kernel);

    void Start();
    void Stop();

    bool IsRunning() const;
        
protected:
    swganh::app::SwganhKernel* kernel();

    anh::ActiveObject& active();

    /*
    * @brief used to subscribe to events on a serivce
    */
    virtual void subscribe() {}
    /*
    * @brief used to perform any startup specific tasks for the service
    */
    virtual void onStart() {}

    /*
    * @brief used to perform any shutdown specific tasks for the service
    */
    virtual void onStop() {}

private:
    BaseService();
    swganh::app::SwganhKernel* kernel_;
        
    std::atomic<bool> running_;

    anh::ActiveObject active_;
};

}}  // swganh::base
#endif // SWGANH_BASE_BASE_SERVICE_H_
