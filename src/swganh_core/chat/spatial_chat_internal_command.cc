// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "spatial_chat_internal_command.h"

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include "swganh/logger.h"
#include "swganh/service/service_manager.h"

#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/tangible/tangible.h"

#include "chat_service.h"

using swganh::app::SwganhKernel;
using swganh::chat::ChatService;
using swganh::chat::SpatialChatInternalCommand;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandCallback;
using swganh::command::CommandProperties;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::object::ObjectController;

using swganh::object::Tangible;

#ifdef WIN32
using std::wregex;
using std::wsmatch;
using std::regex_match;
#else
using boost::wregex;
using boost::wsmatch;
using boost::regex_match;
#endif

SpatialChatInternalCommand::SpatialChatInternalCommand(
    SwganhKernel* kernel,
    const CommandProperties& properties)
    : BaseSwgCommand(kernel, properties)
{
    chat_service_ = kernel->GetServiceManager()->GetService<ChatService>("ChatService");
}

SpatialChatInternalCommand::~SpatialChatInternalCommand()
{}

boost::optional<std::shared_ptr<CommandCallback>> SpatialChatInternalCommand::Run()
{
    // This regular expression searches for 5 numbers separated by spaces
    // followed by a string text message.
    const wregex p(L"(\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (.*)");
    wsmatch m;

    if (regex_match(GetCommandString(), m, p)) {
        chat_service_->SendSpatialChat(
            GetActor(), 
            GetTarget(), 
            m[6].str().substr(0, 256), // Use a max of 255 characters and discard the rest.
            std::stoi(m[2].str()), // Convert the string to an integer 
            std::stoi(m[3].str()));
    }

    return boost::optional<std::shared_ptr<CommandCallback>>();
}
