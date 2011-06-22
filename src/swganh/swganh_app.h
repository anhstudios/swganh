/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team*/

#ifndef SWGANH_SWGANH_APP_H_
#define SWGANH_SWGANH_APP_H_

#include "anh/application.h"

namespace anh {
namespace module_manager { 
    class PlatformServices; 
}}  // namespace anh::module_manager

namespace swganh {

class SwganhApp : public anh::BaseApplication {
public:
    /*! \brief Start up the SharedServices Application which controls everything on a planet.
    *
    *
    */
    explicit SwganhApp(int argc, char* argv[], std::list<std::string> config_files
        , std::shared_ptr<anh::module_manager::PlatformServices> platform_services);

    // overrides
    virtual bool hasStarted() const { return started_; }
    virtual void onAddDefaultOptions_();
    virtual void onRegisterApp_();

    ~SwganhApp();
private:
    /* Disable the default constructor, copy constructor and assignment operators */
    SwganhApp();
    SwganhApp(const SwganhApp&);
    SwganhApp& operator=(const SwganhApp&);
};

}  // namespace swganh

#endif  // SWGANH_SWGANH_APP_H_
