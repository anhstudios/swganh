// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_COMMAND_FACTORY_H_
#define PUB14_CORE_COMMAND_COMMAND_FACTORY_H_

#include <map>
#include <boost/thread/mutex.hpp>

#include "swganh/command/command_factory_interface.h"

namespace swganh {
namespace app {
    class SwganhKernel;
}}

namespace pub14_core {
namespace command {
    
    class CommandFactory : public swganh::command::CommandFactoryInterface
    {
    public:
        ~CommandFactory();

        virtual void AddCommandCreator(anh::HashString command, swganh::command::CommandCreator&& creator);

        virtual void RemoveCommandCreator(anh::HashString command);

        virtual std::unique_ptr<swganh::command::CommandInterface> CreateCommand(
            swganh::app::SwganhKernel* kernel,
            const swganh::command::CommandProperties& properties,
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            const swganh::messages::controllers::CommandQueueEnqueue& command_request);

    private:        
        typedef std::map<
            anh::HashString, 
            swganh::command::CommandCreator
        > CreatorMap;

        boost::mutex creators_mutex_;
        CreatorMap command_creators_;        
    };

}}

#endif  // PUB14_CORE_COMMAND_COMMAND_FACTORY_H_
