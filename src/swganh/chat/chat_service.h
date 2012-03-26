
#ifndef SWGANH_CHAT_CHAT_SERVICE_H_
#define SWGANH_CHAT_CHAT_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/base/base_service.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

namespace swganh {
namespace object {
    namespace creature { class Creature; }
    namespace tangible { class Tangible; }
}}  // namespace swganh::object

namespace swganh {
namespace chat {

    class ChatService: public swganh::base::BaseService
    {
    public:
        explicit ChatService(swganh::app::SwganhKernel* kernel);
        
        anh::service::ServiceDescription GetServiceDescription();

    private:        
        void HandleSpatialChatInternal(
		    const std::shared_ptr<swganh::object::creature::Creature>& actor, // creature object
		    const std::shared_ptr<swganh::object::tangible::Tangible>& target,	// target object
            const swganh::messages::controllers::CommandQueueEnqueue& command);

        void SendSpatialChat(
		    const std::shared_ptr<swganh::object::creature::Creature>& actor, // creature object
		    const std::shared_ptr<swganh::object::tangible::Tangible>& target,	// target object
            std::wstring chat_message,
            uint16_t chat_type,
            uint16_t mood);

        void onStart();
    };

}}  // namespace swganh::chat

#endif  // SWGANH_CHAT_CHAT_SERVICE_H_
