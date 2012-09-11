// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "swganh_core/messages/controllers/show_fly_text.h"

namespace swganh {
namespace object {
	class Object;
}
namespace messages {
	class OutOfBand;
/*
 * A helper class that builds up and sends a System Message
 */
class SystemMessage
{
public:
	static void Send(std::shared_ptr<swganh::object::Object> sender, const std::wstring& custom_message, bool chatbox_only=true, bool send_to_inrange=true);
	static void Send(std::shared_ptr<swganh::object::Object> sender, const swganh::messages::OutOfBand& out_of_band, bool chatbox_only=true, bool send_in_range=true);
	static void Send(std::shared_ptr<swganh::object::Object> sender, const std::wstring& custom_message,const swganh::messages::OutOfBand& out_of_band, bool chatbox_only=true, bool send_in_range=true);
	static void Send(std::shared_ptr<swganh::object::Object> sender, std::string filename, std::string label);
	static void FlyText(std::shared_ptr<swganh::object::Object> sender, const std::string& fly_text, controllers::FlyTextColor color, bool display_flag=false, uint8_t red=0, uint8_t green=0, uint8_t blue=0);
};


}} // swganh::messages