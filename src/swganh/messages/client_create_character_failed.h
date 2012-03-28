// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CLIENT_CREATE_CHARACTER_FAILED_H_
#define SWGANH_MESSAGES_CLIENT_CREATE_CHARACTER_FAILED_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct ClientCreateCharacterFailed : public BaseSwgMessage<ClientCreateCharacterFailed>
    {
        static uint16_t Opcount() { return 3; }
        static uint32_t Opcode() { return 0xDF333C6E; }
        
        std::wstring unk1;
        std::string stf_file;
        std::string error_string;

        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(unk1);
            buffer.write(stf_file);
            buffer.write(0);
            buffer.write(error_string);
        }

        void OnDeserialize(anh::ByteBuffer buffer)
        {
        	unk1 = buffer.read<std::wstring>();
            stf_file = buffer.read<std::string>();
            buffer.read<uint32_t>();
            error_string = buffer.read<std::string>();
        }
    };

}}  // namespace swganh::messages

#endif  // SWGANH_CHARACTER_MESSAGES_CLIENT_CREATE_CHARACTER_FAILED_H_
