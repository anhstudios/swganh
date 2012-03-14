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


#ifndef SWGANH_GLM_BINDING_H_
#define SWGANH_GLM_BINDING_H_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <boost/python.hpp>

using namespace boost::python;
using namespace std;

struct Vec3Wrapper: glm::vec3, wrapper<glm::detail::tvec3<float>>
{
};

void exportVec3()
{
	class_<Vec3Wrapper, boost::noncopyable>("Vec3")
		.def_readwrite("x", &Vec3Wrapper::x, "the x coordinate")
		.def_readwrite("y", &Vec3Wrapper::y, "the y coordinate")
		.def_readwrite("z", &Vec3Wrapper::z, "the z coordinate")
		;
}

struct QuatWrapper: glm::quat, wrapper<glm::detail::tquat<float>>
{
};

void exportQuat()
{
	class_<QuatWrapper, boost::noncopyable>("Quat")
		.def_readwrite("x", &QuatWrapper::x, "the x coordinate")
		.def_readwrite("y", &QuatWrapper::y, "the y coordinate")
		.def_readwrite("z", &QuatWrapper::z, "the z coordinate")
		.def_readwrite("w", &QuatWrapper::w, "the w coordinate")
		;
}

#endif //  SWGANH_GLM_BINDING_H_