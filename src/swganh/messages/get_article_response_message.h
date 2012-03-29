// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_GET_ARTICLE_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_GET_ARTICLE_RESPONSE_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct GetArticleResponseMessage : public BaseSwgMessage<GetArticleResponseMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x934BAEE0; }
    	
    	uint32_t article_text_flag; // 0 = display the article text, 1 = article unable to be retrieved
    	std::wstring article_text;
    
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(article_text_flag);
    		buffer.write(article_text);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		article_text_flag = buffer.read<uint32_t>();
    		article_text = buffer.read<std::wstring>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_GET_ARTICLE_RESPONSE_MESSAGE_H_
