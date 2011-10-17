
#ifndef SWGANH_MESSAGES_DELTAS_MESSAGE_H_
#define SWGANH_MESSAGES_DELTAS_MESSAGE_H_

#include <cstdint>

#include "anh/byte_buffer.h"

#include "swganh/messages/base_deltas_message.h"

namespace swganh {
namespace messages {
    
class DeltasMessage : public BaseDeltasMessage<DeltasMessage> {
public:
    static uint16_t opcount() { return 5; }
    static uint32_t opcode() { return 0x12862153; }
};
    
}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_DELTAS_MESSAGE_H_
