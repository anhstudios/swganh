

#ifndef PACKETS_BASE_SWG_MESSAGE_H_
#define PACKETS_BASE_SWG_MESSAGE_H_

namespace packets {
  
class BaseSwgMessage {
public:
    virtual uint16_t opcount() const = 0;
    virtual uint32_t opcode() const = 0;
    
    void serialize(anh::ByteBuffer& buffer) const {
        buffer.write(opcount());
        buffer.write(opcode());
        
        onSerialize(buffer);
    }
    
    void deserialize(anh::ByteBuffer buffer) {
        uint16_t opcount = buffer.read<uint16_t>();
        uint32_t opcode = buffer.read<uint32_t>();
        
        onDeserialize(std::move(buffer));
    }    
    
protected:
    
    virtual void onSerialize(anh::ByteBuffer& buffer) const = 0;
    virtual void onDeserialize(anh::ByteBuffer buffer) = 0;
};
        
}  // namespace packets

#endif  // PACKETS_BASE_SWG_MESSAGE_H_
