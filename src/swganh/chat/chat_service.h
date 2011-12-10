
#ifndef SWGANH_CHAT_CHAT_SERVICE_H_
#define SWGANH_CHAT_CHAT_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/base/base_service.h"

namespace swganh {
namespace object {
    class Object;
}}  // namespace swganh::object

namespace swganh {
namespace chat {

    class ChatService: public swganh::base::BaseService
    {
    public:
        explicit ChatService(anh::app::KernelInterface* kernel);
        
        anh::service::ServiceDescription GetServiceDescription();

    private:        
        void HandleSpatialChatInternal(
            uint64_t object_id,
            uint64_t target_id,
            std::wstring command_options);

        void SendSpatialChat(
            std::shared_ptr<swganh::object::Object> speaker,
            std::shared_ptr<swganh::object::Object> target,
            std::wstring chat_message,
            uint16_t chat_type,
            uint16_t mood);

        void onStart();
    };

}}  // namespace swganh::chat

#endif  // SWGANH_CHAT_CHAT_SERVICE_H_
