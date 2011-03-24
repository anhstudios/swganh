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

#ifndef ANH_MODULE_MANAGER_MODULE_H_
#define ANH_MODULE_MANAGER_MODULE_H_

#include <string>

namespace anh {
namespace module_manager {

/**
 * 
 */
class Module
{
public:
	/**
	 * Default constructor.
	 *
	 * @arg name Name of the module. (e.g. "Brainwashing Service")
	 * @arg version Version of the module. (e.g. "0.1.5 Beta")
	 * @arg description Description of the module.
	 */
	Module(const std::string& name, const std::string& version, const std::string& description) { };

	/**
	 * Default destructor.
	 */
	~Module() { };

protected:
private:
	std::string _name;
	std::string _version;
	std::string _description;

};

} // namespace module_manager
} // namespace anh

#endif