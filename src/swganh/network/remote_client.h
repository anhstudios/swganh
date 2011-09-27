
#ifndef SWGANH_NETWORK_REMOTE_CLIENT_H_
#define SWGANH_NETWORK_REMOTE_CLIENT_H_

#include <memory>

#include "anh/byte_buffer.h"
#include "anh/network/soe/session.h"

namespace swganh {
namespace network {
    
    class RemoteClient
    {
    public:
        explicit RemoteClient(
            const std::shared_ptr<anh::network::soe::Session>& session);
        
        template<typename T>
        void Send(const T& message)
        {
            session_->SendMessage(message);
        }

        const std::shared_ptr<anh::network::soe::Session>& GetSession() const;
    
    private:
        std::shared_ptr<anh::network::soe::Session> session_;
    };
    
}}  // namespace swganh::network

#endif  // SWGANH_NETWORK_REMOTE_CLIENT_H_
