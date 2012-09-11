// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#include "system_message.h"
#include "out_of_band.h"
#include "chat_system_message.h"
#include "swganh_core/object/object.h"

#ifndef WIN32
#include <boost/regex.hpp>
	using boost::regex;
	using boost::regex_search;
	using boost::smatch;
#else
#include <regex>
#endif

using namespace std;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;

void SystemMessage::Send(
	std::shared_ptr<swganh::object::Object> sender, 
	std::string filename, std::string label)
{
	swganh::messages::OutOfBand prose(filename, label);
    Send(sender, L"", prose, false, false);
}

void SystemMessage::Send(
	std::shared_ptr<swganh::object::Object> sender, 
	const std::wstring& custom_message, 
	bool chatbox_only, bool send_to_inrange)
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

        Send(sender, L"", OutOfBand(file, string), chatbox_only, send_to_inrange);
    }

    Send(sender, custom_message, OutOfBand(),chatbox_only, send_to_inrange);
}
void SystemMessage::Send(
	std::shared_ptr<swganh::object::Object> sender, 
	const swganh::messages::OutOfBand& out_of_band, 
	bool chatbox_only, bool send_in_range)
{
	Send(sender, L"", out_of_band, chatbox_only, send_in_range);
}
void SystemMessage::Send(
	std::shared_ptr<swganh::object::Object> sender, 
	const std::wstring& custom_message,
	const swganh::messages::OutOfBand& out_of_band, 
	bool chatbox_only, bool send_in_range)
{
	uint8_t chatbox = chatbox_only == true ? 2 : 0;
    ChatSystemMessage SystemMessage;

    SystemMessage.message = custom_message;
    SystemMessage.display_type = chatbox;
    if (out_of_band.Length() > 0)
    {
        SystemMessage.AddProsePackage(out_of_band.Pack());
    }

    if (send_in_range)
        sender->NotifyObservers(&SystemMessage);
    else
        sender->GetController()->Notify(&SystemMessage);
}

void SystemMessage::FlyText(
	std::shared_ptr<swganh::object::Object> sender, 
	const std::string& fly_text, controllers::FlyTextColor color, 
	bool display_flag, uint8_t red, uint8_t green, uint8_t blue)
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
        fly_text.object_id = sender->GetObjectId();
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
        sender->NotifyObservers(&fly_text);
    }
}