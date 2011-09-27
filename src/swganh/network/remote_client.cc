
#include "swganh/network/remote_client.h"

using namespace anh::network::soe;
using namespace std;
using namespace swganh::network;

RemoteClient::RemoteClient(const shared_ptr<Session>& session)
    : session_(session)
{}
        
const shared_ptr<Session>& RemoteClient::GetSession() const
{
    return session_;
}
