
#include "swganh/chat/chat_service.h"

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include "anh/logger.h"

#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/messages/controllers/spatial_chat.h"
#include "swganh/messages/obj_controller_message.h"

#include "swganh/object/object.h"
#include "swganh/object/object_controller.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/player/player.h"
#include "swganh/object/tangible/tangible.h"

#include "swganh/command/command_service.h"
#include "swganh/simulation/simulation_service.h"

using namespace anh::app;
using namespace anh::service;
using namespace std;
using namespace swganh::chat;
using namespace swganh::command;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::simulation;

using swganh::app::SwganhKernel;

#ifdef WIN32
using std::wregex;
using std::wsmatch;
using std::regex_match;
#else
using boost::wregex;
using boost::wsmatch;
using boost::regex_match;
#endif

ChatService::ChatService(SwganhKernel* kernel)
: BaseService(kernel)
{}

ServiceDescription ChatService::GetServiceDescription()
{
    ServiceDescription service_description(
        "ChatService",
        "chat",
        "0.1",
        "127.0.0.1", 
        0, 
        0, 
        0);

    return service_description;
}

void ChatService::HandleSpatialChatInternal(
	const std::shared_ptr<swganh::object::creature::Creature>& actor, // creature object
	const std::shared_ptr<swganh::object::tangible::Tangible>& target,	// target object
    const swganh::messages::controllers::CommandQueueEnqueue& command)
{
    // This regular expression searches for 5 numbers separated by spaces
    // followed by a string text message.
    const wregex p(L"(\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (.*)");
    wsmatch m;

    if (! regex_match(command.command_options, m, p)) {
        LOG(error) << "Invalid spatial chat message format";
        return; // We suffered an unrecoverable error, bail out now.
    }
    
    SendSpatialChat(
        actor, 
        target, 
        m[6].str().substr(0, 256),
        std::stoi(m[2].str()),
        std::stoi(m[3].str()));
}

void ChatService::SendSpatialChat(
	const std::shared_ptr<swganh::object::creature::Creature>& actor, // creature object
	const std::shared_ptr<swganh::object::tangible::Tangible>& target,	// target object
    wstring chat_message,
    uint16_t chat_type,
    uint16_t mood)
{    
    SpatialChat spatial_chat;
    spatial_chat.speaker_id = actor->GetObjectId();
    
    if (target)
    {
        spatial_chat.target_id = target->GetObjectId();
    }

    spatial_chat.message = chat_message;
    spatial_chat.chat_type = chat_type;
    spatial_chat.mood = mood;

    spatial_chat.language = static_cast<uint8_t>(0);
    
    actor->NotifyObservers(ObjControllerMessage(0x0000000B, spatial_chat));
}

void ChatService::onStart()
{
	auto command_service = kernel()->GetServiceManager()->GetService<swganh::command::CommandService>("CommandService");
    
    command_service->SetCommandHandler(0x7C8D63D4,
        [this] (
        anh::app::KernelInterface* kernel,
		const std::shared_ptr<swganh::object::creature::Creature>& actor, // creature object
		const std::shared_ptr<swganh::object::tangible::Tangible>& target,	// target object
        const swganh::messages::controllers::CommandQueueEnqueue& command)
    {
        HandleSpatialChatInternal(actor, target, command);
    });
}
