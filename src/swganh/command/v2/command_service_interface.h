// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_SERVICE_INTERFACE_
#define SWGANH_COMMAND_COMMAND_SERVICE_INTERFACE_

#include <cstdint>
#include <memory>

#include "anh/service/service_interface.h"

namespace anh {
namespace observer {
    class ObserverInterface;
}}  // namespace anh::observer


namespace swganh {
namespace command {
namespace v2 {

    class CommandInterface;
    
    class CommandServiceInterface : public anh::service::ServiceInterface
    {
    public:

        void AddAutoCommand(uint64_t object_id, std::unique_ptr<CommandInterface> command);
        
        void RemoveAutoCommand(uint64_t object_id);
                
        void SendCommandQueueRemove(
            std::shared_ptr<anh::observer::ObserverInterface> observer,
            uint32_t action_counter,
            float timer,
            uint32_t error,
            uint32_t action);
    };

}}}  // namespace swganh::command::v2

#endif  // SWGANH_COMMAND_COMMAND_SERVICE_INTERFACE_
