// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CHAT_ROOM_MESSAGE_H_
#define SWGANH_MESSAGES_CHAT_ROOM_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    class ChatRoomMessage : public BaseSwgMessage<ChatRoomMessage>
    {
    public:
    	static uint16_t Opcount() { return 5; }
    	static uint32_t Opcode() { return 0xCD4CE444; }

    	std::string game_name; // arbitrary: "SWG"
    	std::string server_name; // galaxy name
    	std::string sender_character_name;
    	uint32_t channel_id;
    	std::wstring message;
    	std::wstring out_of_band; // apparently chat room messages usually don't send OutOfBand packages, but implementing just in case

    	ChatRoomMessage()
    		: game_name("SWG")
    	{}

    	void AddProsePackage(const anh::ByteBuffer* prose_package)
        {
            prose_package_ = prose_package;
        }

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(game_name);
    		buffer.write(server_name);
    		buffer.write(sender_character_name);
    		buffer.write(channel_id);
    		buffer.write(message);
    		buffer.write(out_of_band);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		game_name = buffer.read<std::string>();
    		server_name = buffer.read<std::string>();
    		sender_character_name = buffer.read<std::string>();
    		channel_id = buffer.read<uint32_t>();
    		message = buffer.read<std::wstring>();
    		out_of_band = buffer.read<std::wstring>();
    	}

    private:
    	const anh::ByteBuffer* prose_package_;
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_ROOM_MESSAGE_H_