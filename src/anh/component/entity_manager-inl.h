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

#ifndef LIBANH_COMPONENT_ENTITY_MANAGER_INL_H_
#define LIBANH_COMPONENT_ENTITY_MANAGER_INL_H_

template<class T> std::shared_ptr<T> EntityManager::QueryInterface(const EntityId& id, const InterfaceType& type)
{
	EntityMapIterator i = entities_.find(id);
	if(i != entities_.end())
		return i->second->QueryInterface<T>(type);
	else
		return T::NullComponent;
}

#endif