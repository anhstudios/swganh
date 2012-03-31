
#include "swganh/object/object_controller.h"

#include "swganh/messages/obj_controller_message.h"
#include "swganh/messages/out_of_band.h"
#include "swganh/messages/chat_system_message.h"
#include "swganh/messages/controllers/show_fly_text.h"
#include "swganh/connection/connection_client.h"
#include "swganh/object/object.h"

#ifndef WIN32
#include <boost/regex.hpp>
    using boost::regex;
    using boost::regex_search;
    using boost::smatch;
#else
    #include <regex>
#endif

using namespace std;
using namespace swganh::connection;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;

ObjectController::ObjectController(
    shared_ptr<Object> object,
    shared_ptr<ConnectionClient> client)
    : object_(object)
    , client_(client)
{
}

ObjectController::~ObjectController()
{
    object_->ClearController();
}

uint64_t ObjectController::GetId() const
{
    if (!object_)
    {
        return 0;
    }

    return object_->GetObjectId();
}

shared_ptr<Object> ObjectController::GetObject() const
{
    return object_;
}

shared_ptr<ConnectionClient> ObjectController::GetRemoteClient()
{
    return client_;
}

void ObjectController::SetRemoteClient(shared_ptr<ConnectionClient> remote_client)
{
    client_ = remote_client;
}

void ObjectController::Notify(const anh::ByteBuffer& message)
{
    client_->SendTo(message);
}

bool ObjectController::SendSystemMessage(const wstring& custom_message, bool chatbox_only, bool send_to_inrange)
{
    // Use regex to check if the chat string matches the stf string format.
    static const regex pattern("@([a-zA-Z0-9/_]+):([a-zA-Z0-9_]+)");
    smatch result;

    string stf_string(custom_message.begin(), custom_message.end());

    // If it's an exact match (2 sub-patterns + the full string = 3 elements) it's an stf string.
    // Reroute the call to the appropriate overload.
    if (regex_search(stf_string, result, pattern))
    {
        string file(result[1].str());
        string string(result[2].str());

        return SendSystemMessage_(L"", OutOfBand(file, string), chatbox_only, send_to_inrange);
    }

    return SendSystemMessage_(custom_message, OutOfBand(),chatbox_only, send_to_inrange);
}

bool ObjectController::SendSystemMessage(const swganh::messages::OutOfBand& prose, bool chatbox_only, bool send_to_inrange)
{
    return SendSystemMessage_(L"", prose, chatbox_only, send_to_inrange);
}

bool ObjectController::SendSystemMessage_(const wstring& custom_message, const OutOfBand& prose, bool chatbox_only, bool send_to_inrange)
{
    uint8_t chatbox = chatbox_only == true ? 2 : 0;
    ChatSystemMessage SystemMessage;

    SystemMessage.message = custom_message;
    SystemMessage.display_type = chatbox;
    if (prose.Length() > 0)
    {
        SystemMessage.AddProsePackage(prose.Pack());
    }

    if (send_to_inrange)
        object_->NotifyObservers(SystemMessage);
    else
        Notify(SystemMessage);

    return true;
}

void ObjectController::SendFlyText(const std::string& fly_text, FlyTextColor color, bool display_flag, uint8_t red, uint8_t green, uint8_t blue)
{
    // Use regex to check if the chat string matches the stf string format.
    static const regex pattern("@([a-zA-Z0-9/_]+):([a-zA-Z0-9_]+)");
    smatch result;

    // If it's an exact match (2 sub-patterns + the full string = 3 elements) it's an stf string.
    // Reroute the call to the appropriate overload.
    if (regex_search(fly_text, result, pattern))
    {
        string file(result[1].str());
        string string(result[2].str());

        controllers::ShowFlyText fly_text;
        fly_text.object_id = object_->GetObjectId();
        fly_text.stf_location = file;
        fly_text.text = string;
        switch (color)
        {
            case RED:
                fly_text.red = 0xFF;
                break;
            case GREEN:
                fly_text.green = 0xFF;
                break;
            case BLUE:
                fly_text.blue = 0xFF;
                break;
            case MIX:
                fly_text.red = red;
                fly_text.green = green;
                fly_text.blue = blue;
                break;
            case WHITE:
                fly_text.red = 0xFF;
                fly_text.green = 0xFF;
                fly_text.blue = 0xFF;
            default:
                break;
        }
        fly_text.display_flag = (display_flag == true) ? 0 : 1;
        object_->NotifyObservers(fly_text);

    }
}
void ObjectController::SendFlyText(const std::string& fly_text, FlyTextColor color)
{
    SendFlyText(fly_text, color, true);
}
