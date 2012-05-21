// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CHAT_CHAT_SERVICE_H_
#define SWGANH_CHAT_CHAT_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>

#include "anh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

namespace swganh {
namespace object {
    namespace creature { class Creature; }
    namespace tangible { class Tangible; }
}}  // namespace swganh::object

namespace swganh {
namespace chat {

    class ChatService: public anh::service::ServiceInterface
    {
    public:
        explicit ChatService(swganh::app::SwganhKernel* kernel);
        
        anh::service::ServiceDescription GetServiceDescription();

        void SendSpatialChat(
		    const std::shared_ptr<swganh::object::creature::Creature>& actor, // creature object
		    const std::shared_ptr<swganh::object::tangible::Tangible>& target,	// target object
            std::wstring chat_message,
            uint16_t chat_type,
            uint16_t mood);

        void Startup();

    private:
        swganh::app::SwganhKernel* kernel_;
    };

}}  // namespace swganh::chat

#endif  // SWGANH_CHAT_CHAT_SERVICE_H_
