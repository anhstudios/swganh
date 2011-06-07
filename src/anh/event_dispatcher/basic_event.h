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

#include "anh/byte_buffer.h"
#include "anh/hash_string.h"
#include "anh/event_dispatcher/event_interface.h"

namespace anh {
namespace event_dispatcher {

namespace detail {
    template<typename T>
    struct IsSerializable {
        template<typename U, void(U::*)(anh::ByteBuffer&) const> struct HasSerialize {};
        template<typename U, void(U::*)(anh::ByteBuffer)> struct HasDeserialize {};
        template<typename U> static char Test(HasSerialize<U, &U::serialize>*, HasDeserialize<U, &U::deserialize>*);
        template<typename U> static int Test(...);
        static const bool value = sizeof(Test<T>(nullptr, nullptr)) == sizeof(char);
    };
}
    
template<typename T>
class BasicEvent : public std::decay<T>::type, public EventInterface {
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
        : std::decay<T>::type(std::forward<T>(data))
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

    uint32_t priority() {
        return priority_;
    }

    void priority(uint32_t priority) {
        priority_ = priority;
    }
    
    uint64_t timestamp() {
        return timestamp_;
    }
    
    void timestamp(uint64_t timestamp)  {
        timestamp_ = timestamp;
    }

    /**
    * If the data policy class T implements serialization semantics this method
    * will serialize it's contents into the buffer, otherwise the buffer is left unmodified.
    *
    * @param buffer The source buffer to fill data with.
    */
    void serialize(anh::ByteBuffer& buffer) const {
        serialize<T>(buffer);
    }
    
    /**
    * If the data policy class T implements serialization semantics this method
    * will fill the data policy class with data from the buffer, otherwise the 
    * buffer is left unmodified.
    *
    * @param buffer The source buffer to fill data with.
    */
    void deserialize(anh::ByteBuffer buffer) {       
        deserialize<std::decay<T>::type>(std::move(buffer));
    }

private:

    template<typename T1>
    typename std::enable_if<!detail::IsSerializable<T1>::value, void>::type
    serialize(anh::ByteBuffer& buffer) const {}
    
    template<typename T1>
    typename std::enable_if<detail::IsSerializable<T1>::value, void>::type
    serialize(anh::ByteBuffer& buffer) const {
        T1::serialize(buffer);
    }
    
    template<typename T1>
    typename std::enable_if<!detail::IsSerializable<T1>::value, void>::type
    deserialize(anh::ByteBuffer& buffer) {}
    
    template<typename T1>
    typename std::enable_if<detail::IsSerializable<T1>::value, void>::type
    deserialize(anh::ByteBuffer buffer) {
        T1::deserialize(std::move(buffer));
    }

    EventType type_;
    uint64_t timestamp_;
    uint32_t priority_;
};

struct NullEventData {};

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
