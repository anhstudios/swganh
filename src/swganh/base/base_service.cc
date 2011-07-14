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

#include "swganh/base/base_service.h"

#include "anh/event_dispatcher/event_dispatcher_interface.h"

using namespace anh;
using namespace swganh::base;
using namespace event_dispatcher;
using namespace std;

BaseService::BaseService(shared_ptr<EventDispatcherInterface> event_dispatcher)
{
    this->event_dispatcher(event_dispatcher);
    running_ = false;
}

BaseService::~BaseService() {}

void BaseService::DescribeConfigOptions(boost::program_options::options_description& description) {

}

void BaseService::Start() {
    subscribe();

    running_ = true;

    onStart();

    while(IsRunning()) {
        event_dispatcher_->tick();
        Update();
    }
}

void BaseService::Stop() {
    running_ = false;
    onStop();
}

bool BaseService::IsRunning() const { return running_; }

shared_ptr<EventDispatcherInterface> BaseService::event_dispatcher() {
    return event_dispatcher_;
}

void BaseService::event_dispatcher(shared_ptr<EventDispatcherInterface> event_dispatcher) {
    event_dispatcher_ = event_dispatcher;
}