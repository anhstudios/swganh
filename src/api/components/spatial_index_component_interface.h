/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team
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
#ifndef ANH_API_COMPONENTS_SPATIAL_COMPONENT_INTERFACE_H
#define ANH_API_COMPONENTS_SPATIAL_COMPONENT_INTERFACE_H

#include <anh/component/component_interface.h>
#include <anh/component/base_component.h>
#include <vector>
#include <boost/flyweight.hpp>

using namespace anh::component;
namespace anh { namespace api { namespace components {
enum REGION_TYPE
{
    REGION_CITY				= 1,
    REGION_SPAWN			= 2,
    REGION_BADGE			= 3,
    REGION_MISSION			= 4,
    REGION_ZONE				= 5,
    REGION_CAMP				= 6,
    REGION_MISSIONSPAWN		= 7
};
class NullSpatialComponent;

class SpatialComponentInterface : public BaseComponent {
public:
    SpatialComponentInterface(const ComponentType& type)
		: BaseComponent(ComponentType("Spatial"), type) { }
    virtual std::vector<std::string> regions() = 0;
    virtual const uint16_t spatial_location_id() = 0;
    virtual const uint32_t last_updated_ms() = 0;
    virtual const std::string& region_name(int index) = 0;
    static std::shared_ptr<NullSpatialComponent> NullComponent;
};

class NullSpatialComponent : SpatialComponentInterface {
public:
    NullSpatialComponent()
		: SpatialComponentInterface("") { }
    std::vector<std::string> regions() { return regions_; }
    const uint16_t spatial_location_id() { return spatial_location_id_; }
    const uint32_t last_updated_ms() { return last_updated_ms_; }
    const std::string& region_name(int index) { return regions_[index]; }
    
private:
    std::vector<std::string> regions_;
    boost::flyweight<uint16_t> spatial_location_id_;
    boost::flyweight<uint32_t> last_updated_ms_;
    
};
} // namespace components
} // namespace api
} // namespace anh
#endif // ANH_API_COMPONENTS_SPATIAL_COMPONENT_INTERFACE_H