#ifndef MOD_ANH_LOGIN_SESSION_H_
#define MOD_ANH_LOGIN_SESSION_H_

#include <cstdint>

namespace network {
    class Session 
    {
    public:
        Session() {}
        virtual ~Session(){}
        uint64_t account_id;
        int authentication_status;
    };
}

#endif //MOD_ANH_LOGIN_SESSION_H_