
#include "swganh/login/base_login_service.h"

using namespace anh::app;
using namespace swganh::base;
using namespace swganh::login;

using namespace std;

BaseLoginService::BaseLoginService(shared_ptr<KernelInterface> kernel) 
    : BaseService(kernel)
#pragma warning(push)
#pragma warning(disable: 4355)
    , SwgMessageRouter([=] (const boost::asio::ip::udp::endpoint& endpoint) {
        return GetClientFromEndpoint(endpoint);  
      })
#pragma warning(pop) 
{}