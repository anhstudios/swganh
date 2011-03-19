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

#include "appearance_component.h"


using namespace std;
namespace zone { namespace components {

// statics
std::shared_ptr<NullAppearanceComponent> AppearanceComponentInterface::NullComponent = std::shared_ptr<NullAppearanceComponent>(new NullAppearanceComponent());
ComponentInfo NullAppearanceComponent::component_info_ = ComponentInfo(ComponentType("NullAppearanceComponent"), false);
ComponentInfo AppearanceComponent::component_info_ = ComponentInfo(ComponentType("AppearanceComponent"), true);

AppearanceComponent::AppearanceComponent(const ObjectId& id)
: AppearanceComponentInterface(id)
{}

void AppearanceComponent::Init(boost::property_tree::ptree& pt) {
    // initial default values
    gender_ = pt.get<bool>("gender", false);
    model_ = pt.get<std::string>("model", ""); 
	name_ = pt.get<std::string>("name", ""); 
    name_file_ = pt.get<std::string>("name_file", "");
    detail_file_ = pt.get<std::string>("detail_file", "");
    color_ = pt.get<std::string>("color", "");
    customization_string_ = pt.get<std::string>("customization_string", "");
}


} // namespace components
} // namespace zone