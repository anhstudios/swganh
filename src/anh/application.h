/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,a
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#ifndef ANH_APPLICATION_H_
#define ANH_APPLICATION_H_

#include <cstdint>
#include <list>
#include <boost/program_options.hpp>

#include <anh/application_interface.h>

namespace sql { class Connection; class Driver; }
namespace anh {
// forward declarations
namespace event_dispatcher { class EventDispatcherInterface; class EventInterface; }
namespace database { class DatabaseManagerInterface; class DatabaseManager; }
namespace scripting { class ScriptingManagerInterface; }
namespace server_directory { class ServerDirectoryInterface; }
namespace module_manager { class ModuleManager; class PlatformServices; }

/**
 * \brief Holds common functionality used between all servers in the cluster.
 *  Including loading base configuration, event dispatching, scripting, database managing
*   and registration with server directory.
 */
class BaseApplication : public ApplicationInterface {
public:
    /**
     * Initializes general configuration options used between all servers in the cluster.
     */
    BaseApplication(
          int argc, char* argv[]
        , std::list<std::string> config_files
        , std::shared_ptr<module_manager::PlatformServices> platform_services);

    BaseApplication(
          std::list<std::string> config_files
        , std::shared_ptr<module_manager::PlatformServices> platform_services);

    BaseApplication(
          int argc, char* argv[]
        , std::shared_ptr<module_manager::PlatformServices> platform_services);
    
    /**
     * Default Deconstructor.
     */
    ~BaseApplication();

    void startup();
    void process();
    void shutdown();

    /**
    *   creates a server directory from sql connection
    *
    *   \return shared_ptr to the ServerDirectory
    */
    virtual std::shared_ptr<server_directory::ServerDirectoryInterface> createServerDirectory(std::shared_ptr<sql::Connection> conn);
    /**
    *   creates a database manager from default configuration
    */
    virtual std::shared_ptr<database::DatabaseManager> createDatabaseManager(sql::Driver* driver);
    /**
    *   \brief Sets up basic logging options for logging
    */
    virtual void setupLogging();

    boost::program_options::variables_map configuration_variables_map() { return configuration_variables_map_; }
    std::shared_ptr<database::DatabaseManagerInterface> database_manager() { return db_manager_; }
    std::shared_ptr<event_dispatcher::EventDispatcherInterface> event_dispatcher() { return event_dispatcher_; }
    std::shared_ptr<scripting::ScriptingManagerInterface> scripting_manager() { return scripting_manager_; }
    std::shared_ptr<server_directory::ServerDirectoryInterface> server_directory() { return server_directory_; }
protected:

    /// helper function to init needed services
    void init_services_();

    /**
    *   adds data source from default configuration loaded in
    *   @addDefaultOptions_ /requires DatabaseManagerInterface
    */

    bool addDataSourcesFromOptions_();
    /**
    *   Adds default configuration options to be used in @loadOptions_
    *   
    */
    void addDefaultOptions_();
    /**
    *   Allows children to add more configuration options after defaults
    *
    */
    virtual void onAddDefaultOptions_() = 0;
    /**
    *   Uses loaded cluster information to register the application with the server directory
    *
    */
    void registerApp_();
    /// allows children to add more data sources, after defaults have been added
    virtual void onRegisterApp_() = 0;
    
    /**
     * Loads configuration options using the configuration options description from
     * the command-line and configuration files.
     *
     * \param argc The number of arugments.
     * \param argv The arguments pulled from the command-line.
     * \param config_files The files to load the options from.
     */
    void loadOptions_(uint32_t argc, char* argv[], std::list<std::string> config_files);

    // base events to be triggered
    std::shared_ptr<event_dispatcher::EventInterface> startup_event_;
    std::shared_ptr<event_dispatcher::EventInterface> process_event_;
    std::shared_ptr<event_dispatcher::EventInterface> shutdown_event_;

    std::shared_ptr<database::DatabaseManagerInterface> db_manager_;
    std::shared_ptr<event_dispatcher::EventDispatcherInterface> event_dispatcher_;
    std::shared_ptr<scripting::ScriptingManagerInterface> scripting_manager_;
    std::shared_ptr<server_directory::ServerDirectoryInterface> server_directory_;
    std::shared_ptr<module_manager::ModuleManager> module_manager_;
    std::shared_ptr<module_manager::PlatformServices> platform_services_;

    boost::program_options::options_description configuration_options_description_;
    boost::program_options::variables_map configuration_variables_map_;
    int argc_;
    char** argv_;
    bool started_;
    std::list<std::string> config_files_;
};

}  // namespace anh

#endif  // ANH_SERVER_H_