/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_SWGANH_APP_H_
#define SWGANH_SWGANH_APP_H_

#include <memory>
#include <string>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <tbb/atomic.h>

namespace anh {
namespace event_dispatcher {
class EventDispatcherInterface;
}  // namespace event_dispatcher

namespace module_manager {
class ModuleManager;
class PlatformServices;
}  // namespace module_manager
}  // namespace anh

namespace swganh {

struct DatabaseConfig {
    std::string host;
    std::string schema;
    std::string username;
    std::string password;
};

struct AppConfig {
    bool single_server_mode;
    std::vector<std::string> modules;

    std::string galaxy_name;

    DatabaseConfig galaxy_manager_db;
    DatabaseConfig galaxy_db;

    boost::program_options::options_description BuildConfigDescription();
};

typedef std::pair<boost::program_options::options_description, boost::program_options::variables_map> ModuleConfig;

class SwganhApp : private boost::noncopyable {
public:
    SwganhApp();    
    ~SwganhApp();
    
    void event_dispatcher(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher);
        
    void Initialize(int argc, char* argv[]);

    void Start();

    void Stop();

    bool IsRunning();

private:
    void LoadAppConfig_(int argc, char* argv[], AppConfig& app_config);
    void LoadModuleConfig_(ModuleConfig& module_config);

    void LoadModules_(std::vector<std::string> modules);

    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher_;
    std::shared_ptr<anh::module_manager::ModuleManager>              module_manager_;
    std::shared_ptr<anh::module_manager::PlatformServices>           platform_services_;

    tbb::atomic<bool> running_;
    bool initialized_;
    
    AppConfig app_config_;
    ModuleConfig module_config_;
};

}  // namespace swganh

#endif  // SWGANH_SWGANH_APP_H_
