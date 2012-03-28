/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2012 The SWG:ANH Team

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

#ifndef QUADTREE_SPATIAL_PROVIDER_H_
#define QUADTREE_SPATIAL_PROVIDER_H_

#include "swganh/simulation/spatial_provider_interface.h"

class QuadtreeSpatialProvider : public swganh::simulation::SpatialProviderInterface
{
public:
	QuadtreeSpatialProvider(anh::app::KernelInterface* kernel);
	virtual ~QuadtreeSpatialProvider(void);

	virtual void AddObject(std::shared_ptr<swganh::object::Object> obj);
	virtual void RemoveObject(std::shared_ptr<swganh::object::Object> obj);
	virtual void UpdateObject(std::shared_ptr<swganh::object::Object> obj);

	virtual std::vector<std::shared_ptr<swganh::object::Object>> GetObjectsInRange(std::shared_ptr<swganh::object::Object>);
	virtual std::vector<std::shared_ptr<swganh::object::Object>> GetObjectsInRange(std::shared_ptr<swganh::object::Object>, float);
};

#endif // QUADTREE_SPATIAL_PROVIDER_H_