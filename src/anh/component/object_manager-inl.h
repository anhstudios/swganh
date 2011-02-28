/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBANH_COMPONENT_OBJECT_MANAGER_INL_H_
#define LIBANH_COMPONENT_OBJECT_MANAGER_INL_H_

namespace anh {
namespace component {

template<class T> std::shared_ptr<T> ObjectManager::QueryInterface(const ObjectId& id, const ComponentType& type)
{
	std::shared_ptr<T> component(T::NullComponent);

	// Find out object.
	ObjectComponentMapIterator i = object_components_map_.find(id);
	if(i != object_components_map_.end())
	{
		// Seach for the correct interface.
		std::for_each((*i).second.begin(), (*i).second.end(), [=, &component](std::shared_ptr<ComponentInterface> comp) {
			if(comp->component_info().type == type) {
				component = std::static_pointer_cast<T>(comp);
				return;
			}
		});
	}

	return component;
}

}
}

#endif