/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team*/

#ifndef CONNECTION_APP_H_
#define CONNECTION_APP_H_

#include <anh/application.h>
#include <anh/network/cluster/service.h>

namespace anh {
namespace module_manager { class PlatformServices; }
namespace component { class EntityBuilder; class EntityManager; }
}

namespace connection {
class ConnectionApp : public anh::BaseApplication
{
public:
    /*! @brief Start up the Connection Application which controls the handling of 
    *   all messages to and from the zone and clients
    *
    *
    */
    explicit ConnectionApp(int argc, char* argv[], std::list<std::string> config_files
        , std::shared_ptr<anh::module_manager::PlatformServices> platform_services);

    // overrides
    virtual void startup();
    virtual void update();
    virtual void shutdown();
    virtual bool hasStarted() const { return started_; }
    virtual void onAddDefaultOptions_();
    virtual void onRegisterApp_();

    ~ConnectionApp();
private:
    /* Disable the default constructor, copy constructor and assignment operators */
    ConnectionApp();
    ConnectionApp(const ConnectionApp&);
    ConnectionApp& operator=(const ConnectionApp&);

    anh::network::cluster::Service           cluster_service_;
};
} // end namespace zone
#endif // CONNECTION_APP_H_