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

#ifndef ANH_DATABASE_DATABASE_MANAGER_INTERFACE_H_
#define ANH_DATABASE_DATABASE_MANAGER_INTERFACE_H_

#include <cstdint>
#include <memory>
#include <string>

#include <boost/noncopyable.hpp>
#include "anh/hash_string.h"

namespace sql {
    class Connection;
    class Driver;
}
  
/*! An identifier used to label different persistant data storage types.
*/
typedef anh::HashString StorageType;

namespace anh {
namespace database {
/*! Interface class that exposes an API for managing mysql connector/c++
* connections.
*/
class DatabaseManagerInterface {
public:    
    virtual ~DatabaseManagerInterface() {}

    /*! Check to see whether a specified storage type has been registered with 
    * the DatabaseManager instance or not.
    *
    * @return bool True if the storage type has been registered, false if not.
    */
    virtual bool hasStorageType(const StorageType& storage_type) const = 0;

    /*! Registers a storage type with the DatabaseManager. This creates a connection
    * to the datastore to validate the settings, which is then immediately placed
    * in the connection pool for use.
    *
    * @return bool True if the storage type was registered, false if registration already exist.
    */
    virtual bool registerStorageType(const StorageType& storage_type, const std::string& schema, const std::string& host, const std::string& username, const std::string& password) = 0;

    /*! Check to see whether a connection exists already for a given storage type.
    *
    * @return bool True if a connection for the given storage type exists, false if not.
    */
    virtual bool hasConnection(const StorageType& storage_type) const = 0;

    /*! Processes a request for a connection to a specific storage type. 
    *
    * @param storage_type The storage type a connection is being requested for.
    * @return Returns a connection or nullptr if one could not be created, or if
    *   the storage type has not been seen before.
    */
    virtual std::shared_ptr<sql::Connection> getConnection(const StorageType& storage_type) = 0;
};
} // database
} // anh
#endif //ANH_DATABASE_DATABASE_MANAGER_INTERFACE_H_