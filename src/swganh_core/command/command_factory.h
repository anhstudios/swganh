// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <map>
#include <tuple>
#include <boost/thread/mutex.hpp>

#include "swganh_core/command/command_factory_interface.h"

namespace swganh {
namespace app {
    class SwganhKernel;
}
namespace command {
    class CommandServiceInterface;
}}

namespace swganh {
namespace command {
    
	/**
	* Implementation that associates a command name with creation of a command
	*/
    class CommandFactory : public swganh::command::CommandFactoryInterface
    {
    public:
		/**
		* Creates a new instance
		*/
        explicit CommandFactory(swganh::app::SwganhKernel* kernel);

		/**
		* Associates a particular command name with a creator
		* @param command the name
		* @param creator the creator
		*/
        virtual void AddCommandCreator(swganh::HashString command, swganh::command::CommandCreator&& creator);

		/**
		* Disassociates a particular command name with a creator
		* @param command the command name to disassociate
		*/
        virtual void RemoveCommandCreator(swganh::HashString command);

		/**
		* Create a command by name
		*/
        virtual std::shared_ptr<swganh::command::CommandInterface> CreateCommand(swganh::HashString command);

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

        typedef std::map<swganh::HashString, CreatorData> CreatorMap;

        swganh::app::SwganhKernel* kernel_;
        swganh::command::CommandServiceInterface* command_service_;

        boost::mutex creators_mutex_;
        CreatorMap command_creators_;        
    };

}}
