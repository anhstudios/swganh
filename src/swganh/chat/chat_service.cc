
#include "swganh/chat/chat_service.h"

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include <glog/logging.h>

#include "anh/app/kernel_interface.h"
#include "anh/service/service_manager.h"

#include "swganh/messages/controllers/spatial_chat.h"
#include "swganh/messages/obj_controller_message.h"

#include "swganh/object/object.h"
#include "swganh/object/object_controller.h"
#include "swganh/object/player/player.h"

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

#ifdef WIN32
using std::wregex;
using std::wsmatch;
using std::regex_match;
#else
using boost::wregex;
using boost::wsmatch;
using boost::regex_match;
#endif

ChatService::ChatService(KernelInterface* kernel)
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
    uint64_t object_id,
    uint64_t target_id,
    std::wstring command_options)
{
    auto simulation_service = kernel()->GetServiceManager()
        ->GetService<SimulationService>("SimulationService");

    shared_ptr<Object> object = simulation_service->GetObjectById(object_id);
    shared_ptr<Object> target = nullptr;

    if (target_id != 0)
    {
        target = simulation_service->GetObjectById(target_id);
    }

    // This regular expression searches for 5 numbers separated by spaces
    // followed by a string text message.
    const wregex p(L"(\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (.*)");
    wsmatch m;

    if (! regex_match(command_options, m, p)) {
        LOG(ERROR) << "Invalid spatial chat message format";
        return; // We suffered an unrecoverable error, bail out now.
    }
    
    SendSpatialChat(
        object, 
        target, 
        m[6].str().substr(0, 256),
        std::stoi(m[2].str()),
        std::stoi(m[3].str()));
}

void ChatService::SendSpatialChat(
    shared_ptr<Object> speaker,
    shared_ptr<Object> target,
    wstring chat_message,
    uint16_t chat_type,
    uint16_t mood)
{    
    SpatialChat spatial_chat;
    spatial_chat.speaker_id = speaker->GetObjectId();
    
    if (target)
    {
        spatial_chat.target_id = target->GetObjectId();
    }

    spatial_chat.message = chat_message;
    spatial_chat.chat_type = chat_type;
    spatial_chat.mood = mood;

    auto speaker_tmp = static_pointer_cast<player::Player>(speaker);
    spatial_chat.language = static_cast<uint8_t>(0);
    
    speaker->NotifyObservers(ObjControllerMessage(0x0000000B, spatial_chat));
}

void ChatService::onStart()
{
    //auto command_service = kernel()->GetServiceManager()->GetService<swganh::command::CommandService>("CommandService");
    //
    //command_service->AddCommandHandler(0x7C8D63D4,
    //    swganh::scripting::PythonCommand("scripts/commands/spatialchatinternal.py"));
}
