/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team*/

#ifndef ZONE_APP_H_
#define ZONE_APP_H_

#include <anh/application.h>

namespace anh {
namespace module_manager { class PlatformServices; }
namespace component { class EntityBuilder; class EntityManager; }
}

namespace zone {
class ZoneApp : public anh::BaseApplication
{
public:
    /*! \brief Start up the Zone Application which controls everything on a planet.
    *
    *
    */
    explicit ZoneApp(int argc, char* argv[], std::list<std::string> config_files
        , std::shared_ptr<anh::module_manager::PlatformServices> platform_services);

    // overrides
    virtual bool hasStarted() const { return started_; }
    virtual void onAddDefaultOptions_();
    virtual void onRegisterApp_();

    ~ZoneApp();
private:
    /* Disable the default constructor, copy constructor and assignment operators */
    ZoneApp();
    ZoneApp(const ZoneApp&);
    ZoneApp& operator=(const ZoneApp&);

    std::shared_ptr<anh::component::EntityBuilder> object_builder_;
    std::shared_ptr<anh::component::EntityManager> object_manager_;
    std::string zone_name_;

};


} // end namespace zone
#endif // ZONE_APP_H_