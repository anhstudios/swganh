#ifndef ANH_PACKETS_SEARCH_KNOWLEDGE_BASE_MESSAGE_H
#define ANH_PACKETS_SEARCH_KNOWLEDGE_BASE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S ]
namespace packets {
struct SearchKnowledgeBaseMessage : public BasePacket
{
    SearchKnowledgeBaseMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::wstring search_text_ = std::wstring() )
        : BasePacket(session_, ZONE)
    , search_text(search_text_) {}
    std::wstring search_text;
};

class SearchKnowledgeBaseMessageEvent : public anh::event_dispatcher::BasicEvent<SearchKnowledgeBaseMessage>{
public:    
    SearchKnowledgeBaseMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::wstring search_text_ = std::wstring()) 
        : anh::event_dispatcher::BasicEvent<SearchKnowledgeBaseMessage>("SearchKnowledgeBaseMessage"){}
    virtual ~SearchKnowledgeBaseMessageEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        search_text = buffer.read<std::wstring>();
    }
};

} // packets

#endif // ANH_PACKETS_SEARCH_KNOWLEDGE_BASE_MESSAGE_H