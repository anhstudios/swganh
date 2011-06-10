/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team*/

#ifndef LOGIN_APP_H_
#define LOGIN_APP_H_

#include <anh/application.h>
#include <anh/network/soe/service.h>
#include <anh/network/cluster/cluster_service.h>

namespace anh {
namespace module_manager { class PlatformServices; }
}

namespace login {
class LoginApp : public anh::BaseApplication
{
public:
    /*! \brief Start up the Login Application which handles client login
    *
    *
    */
    explicit LoginApp(int argc, char* argv[], std::list<std::string> config_files
        , std::shared_ptr<anh::module_manager::PlatformServices> platform_services);

    // overrides
    virtual bool hasStarted() const { return started_; }
    virtual void onAddDefaultOptions_();
    virtual void onRegisterApp_();
    virtual void startup();
    virtual void process();
    virtual void shutdown();

    ~LoginApp();
private:
    /* Disable the default constructor, copy constructor and assignment operators */
    LoginApp();
    LoginApp(const LoginApp&);
    LoginApp& operator=(const LoginApp&);

    // SOE Frontend Service
    anh::network::soe::Service	soe_service_;
};


} // end namespace login
#endif // LOGIN_APP_H_