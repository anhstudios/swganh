// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/messages/controllers/command_queue_enqueue.h"

namespace swganh {
namespace object {
    class Object;
}}  // namespace swganh::object

namespace swganh {
namespace chat {

    class ChatServiceInterface: public swganh::service::ServiceInterface
    {
    public:
        
        virtual void SendSpatialChat(
		    const std::shared_ptr<swganh::object::Object>& actor, // creature object
		    const std::shared_ptr<swganh::object::Object>& target,	// target object
            std::wstring chat_message,
            uint16_t chat_type,
            uint16_t mood) = 0;
    };

}}  // namespace swganh::chat
