/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_BASE_BASE_SERVICE_H_
#define SWGANH_BASE_BASE_SERVICE_H_

#include "service_interface.h"

#include <memory>
#include <tbb/atomic.h>

#include "anh/event_dispatcher/event_dispatcher_interface.h"

namespace swganh {
namespace base {

class BaseService : public ServiceInterface {
public:
    explicit BaseService(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher);
    ~BaseService();
    
    virtual void Start();
    virtual void Stop();

    virtual bool IsRunning() const;

    virtual void DescribeConfigOptions(boost::program_options::options_description& description);
    /*
    *  @brief sets the event_dispatcher
    */
    virtual void event_dispatcher(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher);
    /*
    *  @brief used to subscribe to events on a serivce
    */
    virtual void subscribe() = 0;
    /*
    *  @brief used to perform any startup specific tasks for the service
    */
    virtual void onStart() = 0;
    /*
    *  @brief used to perform any shutdown specific tasks for the service
    */
    virtual void onStop() = 0;
    /*
    *  @brief used to perform any update specific tasks for the service
    */
    virtual void Update() = 0;
    /*
    *  @brief gets the event_dispatcher
    */
    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher();
    

protected:
    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher_;
    
    tbb::atomic<bool> running_;
private:
    BaseService();
};

}}  // swganh::base
#endif // SWGANH_BASE_BASE_SERVICE_H_