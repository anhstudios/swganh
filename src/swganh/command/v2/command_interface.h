// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_V2_COMMAND_INTERFACE_H_
#define SWGANH_COMMAND_V2_COMMAND_INTERFACE_H_

#include <memory>

namespace swganh {
namespace object {
    class ObjectController;
}  // namespace object

namespace command {
namespace v2 {
    
    /**
     */
    class CommandInterface
    {
    public:
        virtual ~CommandInterface() {}

        virtual const std::shared_ptr<object::ObjectController>& GetController() const;
    };
    
}}}  // namespace swganh::command::v2

#endif  // SWGANH_COMMAND_V2_COMMAND_INTERFACE_H_
