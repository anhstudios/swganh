// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_COMMAND_FACTORY_H_
#define PUB14_CORE_COMMAND_COMMAND_FACTORY_H_

#include <map>
#include <tuple>
#include <boost/thread/mutex.hpp>

#include "swganh/command/command_factory_interface.h"

namespace swganh {
namespace app {
    class SwganhKernel;
}
namespace command {
    class CommandServiceInterface;
}}

namespace pub14_core {
namespace command {
    
    class CommandFactory : public swganh::command::CommandFactoryInterface
    {
    public:
        explicit CommandFactory(swganh::app::SwganhKernel* kernel);
        ~CommandFactory();

        virtual void AddCommandCreator(anh::HashString command, swganh::command::CommandCreator&& creator);

        virtual void RemoveCommandCreator(anh::HashString command);

        virtual std::shared_ptr<swganh::command::CommandInterface> CreateCommand(anh::HashString command);

    private:
        swganh::command::CommandServiceInterface* GetCommandService();

        struct CreatorData
        {
            CreatorData(swganh::command::CommandCreator&& creator_func,
                const swganh::command::CommandProperties&  properties)
                : creator_func(std::move(creator_func))
                , properties(properties)
            {}

            swganh::command::CommandCreator creator_func;
            const swganh::command::CommandProperties& properties;
        };

        typedef std::map<
            anh::HashString, CreatorData
        > CreatorMap;

        swganh::app::SwganhKernel* kernel_;
        swganh::command::CommandServiceInterface* command_service_;

        boost::mutex creators_mutex_;
        CreatorMap command_creators_;        
    };

}}

#endif  // PUB14_CORE_COMMAND_COMMAND_FACTORY_H_
