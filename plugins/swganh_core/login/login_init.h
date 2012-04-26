// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CORE_LOGIN_INITIALIZATION_H_
#define SWGANH_CORE_LOGIN_INITIALIZATION_H_

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "anh/logger.h"

#include "anh/plugin/bindings.h"
#include "anh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "mysql_account_provider.h"
#include "mysql_session_provider.h"
#include "sha512_encoder.h"

namespace swganh_core {
namespace login {

inline void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    anh::plugin::ObjectRegistration registration;
    registration.version.major = 0;
    registration.version.minor = 4;

    // Register
    registration.CreateObject = [kernel] (anh::plugin::ObjectParams* params) -> void * {
        return new Sha512Encoder(kernel->GetDatabaseManager());
    };

    registration.DestroyObject = [] (void * object) {
        if (object) {
            delete static_cast<Sha512Encoder*>(object);
        }
    };

    kernel->GetPluginManager()->RegisterObject("LoginService::Encoder", &registration);
    
    registration.CreateObject = [kernel] (anh::plugin::ObjectParams* params) -> void * {
        return new MysqlAccountProvider(kernel->GetDatabaseManager());
    };

    registration.DestroyObject = [] (void * object) {
        if (object) {
            delete static_cast<MysqlAccountProvider*>(object);
        }
    };
    
    kernel->GetPluginManager()->RegisterObject("LoginService::AccountProvider", &registration);
        
     registration.CreateObject = [kernel] (anh::plugin::ObjectParams* params) -> void * {
         return new MysqlSessionProvider(kernel->GetDatabaseManager());
     };
    
     registration.DestroyObject = [] (void * object) {
         if (object) {
             delete static_cast<MysqlSessionProvider*>(object);
         }
     };
    
    kernel->GetPluginManager()->RegisterObject("ConnectionService::SessionProvider", &registration);
}

}}  // namespace swganh::login

#endif  // SWGANH_CORE_LOGIN_LOGIN_INITIALIZATION_H_
