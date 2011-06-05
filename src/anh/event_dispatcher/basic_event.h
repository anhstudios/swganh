/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBANH_EVENT_DISPATCHER_BASIC_EVENT_H_
#define LIBANH_EVENT_DISPATCHER_BASIC_EVENT_H_

#include <cstdint>
#include <memory>
#include <type_traits>

#include <tbb/spin_rw_mutex.h>

#include "anh/byte_buffer.h"
#include "anh/hash_string.h"
#include "anh/event_dispatcher/event_interface.h"

namespace anh {
namespace event_dispatcher {

namespace detail {
    /**
    * Compile time constraint that ensures that T has serialize and deserialize member functions.
    */
    template<typename T>
    class is_serializable {
    public:
        static void Constraints() {
            void (T::*test1)(anh::ByteBuffer&) const = &T::serialize;
            void (T::*test2)(anh::ByteBuffer) = &T::deserialize;
            test1;
            test2;
        }
        
        is_serializable() { void (*p)() = Constraints; }
    };
}

template<typename T>
class BasicEvent : public EventInterface, detail::is_serializable<T> {
public:
    BasicEvent()
        : type_(std::decay<T>::type::type())
        , timestamp_(0)
        , priority_(std::decay<T>::type::priority()) {}
    
    explicit BasicEvent(EventType type)
        : type_(std::move(type))
        , timestamp_(0)
        , priority_(0) {}
    
    BasicEvent(EventType type, T&& data)
        : data_(std::forward<T>(data))
        , type_(std::move(type))
        , timestamp_(0)
        , priority_(0) {}
    
    BasicEvent(EventType type, uint32_t priority)
        : type_(std::move(type))
        , timestamp_(0)
        , priority_(priority) {}

    ~BasicEvent() {}

    const EventType& type() const { 
        return type_; 
    }

    T data() { 
        tbb::spin_rw_mutex::scoped_lock lock(spin_mutex_, false);
        return data_; 
    }

    void data(T& data ) { 
        tbb::spin_rw_mutex::scoped_lock lock(spin_mutex_, true);
        data_ = data; 
    }

    uint32_t priority() {
        tbb::spin_rw_mutex::scoped_lock lock(spin_mutex_, false);
        return priority_;
    }

    void priority(uint32_t priority) {
        tbb::spin_rw_mutex::scoped_lock lock(spin_mutex_, true);
        priority_ = priority;
    }
    
    uint64_t timestamp() {
        tbb::spin_rw_mutex::scoped_lock lock(spin_mutex_, false);
        return timestamp_;
    }
    
    void timestamp(uint64_t timestamp)  {
        tbb::spin_rw_mutex::scoped_lock lock(spin_mutex_, true);
        timestamp_ = timestamp;
    }

private:
    tbb::spin_rw_mutex spin_mutex_;
    T data_;
    EventType type_;
    uint64_t timestamp_;
    uint32_t priority_;
};

struct NullEventData {
    void serialize(anh::ByteBuffer&) const {}
    void deserialize(anh::ByteBuffer) {}
};

typedef BasicEvent<NullEventData> SimpleEvent;

/**
* Creates an event based on the given name and data.
*
* @param type The type of event to create.
* @param data The data contents of the event to create.
* @returns The newly created event.
*/
template<typename T> inline
BasicEvent<T> make_event(EventType type, T&& data) {
    BasicEvent<T> created_event(type, std::forward<T>(data));
    return created_event;
}

/**
* Creates a simple event with no data.
*
* @param type The type of event to create.
* @returns The newly created event.
*/
SimpleEvent make_event(EventType type);

/**
* Creates an event based on the given name and data in a shared_ptr container.
*
* @param type The type of event to create.
* @param data The data contents of the event to create.
* @returns The newly created event.
*/
template<typename T> inline
std::shared_ptr<BasicEvent<T>> make_shared_event(EventType type, T&& data) {
    auto created_event = std::make_shared<BasicEvent<T>>(type, std::forward<T>(data));
    return created_event;
}

/**
* Creates a simple event with no data in a shared_ptr container.
*
* @param type The type of event to create.
* @returns The newly created event.
*/
std::shared_ptr<SimpleEvent> make_shared_event(EventType type);

}  // namespace event_dispatcher
}  // namespace anh

#endif  // LIBANH_EVENT_DISPATCHER_BASIC_EVENT_H_
