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

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#ifndef ANH_MODULE_MANAGER_PLATFORM_SERVICES_H_
#define ANH_MODULE_MANAGER_PLATFORM_SERVICES_H_

#include <map>
#include <boost/any.hpp>
#include <anh/hash_string.h>

namespace anh {
namespace module_manager {

typedef HashString Service;

/**
 * Aggregates all core systems to the module.
 */
class PlatformServices
{
public:
    /**
     * Default constructor.
     */
    PlatformServices(void);

    /**
     * Default deconstructor.
     */
    ~PlatformServices(void);

    /**
     * Adds a service to the Platform Services.
     */
    bool addService(Service service_name, boost::any service);

    /**
     * Removes a service from the Platform Services.
     */
    void removeService(Service service_name);
    
    template<typename T>
    T getService(Service service_name) {        
        return boost::any_cast<T>(getService_(service_name));
    }

    /**
     * 
     */
    bool hasService(Service service_name);

private:
    typedef std::map<Service, boost::any>			ServiceMap;
    typedef std::map<Service, boost::any>::iterator ServiceMapIterator;
    
    /**
     * Finds and returns a service from the Platform Services.
     */
    boost::any getService_(Service service_name);

    ServiceMap services_;
};

}
}

#endif