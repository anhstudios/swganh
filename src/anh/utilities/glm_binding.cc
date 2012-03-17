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
#include <anh/utility_python.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <boost/python.hpp>
using namespace boost::python;
namespace anh
{
namespace utilities
{
void define_class_glm_vec3()
{
	class_<glm::vec3>("vector3",
		"Stores a direction vector in three-dimensional space", no_init)
		.def(init<glm::float_t, glm::float_t, glm::float_t>())
		.def(init<const glm::vec3&>())
        .def_readwrite("x", &glm::vec3::x)
        .def_readwrite("y", &glm::vec3::y)
        .def_readwrite("z", &glm::vec3::z)
		.def("__len__", &utility::constant_len_len<glm::vec3, 3>)
		.def("__getitem__", &utility::constant_len_get_item<glm::vec3, 3, glm::float_t>)
		.def("__setitem__", &utility::constant_len_set_item<glm::vec3, 3, glm::float_t>)
		.def(self == self)
		.def(self != self)
		.def(self + self)
		.def(self - self)
		.def(self * glm::float_t())
		.def(glm::float_t() * self)
		.def(self += self)
		.def(self -= self)
		.def(self *= glm::float_t())
		.def(self /= glm::float_t());
		//.def(self_ns::str(self));
}
void define_class_glm_quat() 
{
    class_<glm::quat>("quat",
		"Stores a quaternion", no_init)
		.def(init<glm::float_t, glm::float_t, glm::float_t, glm::float_t>())
		.def(init<const glm::quat&>())
        .def_readwrite("x", &glm::quat::x)
        .def_readwrite("y", &glm::quat::y)
        .def_readwrite("z", &glm::quat::z)
        .def_readwrite("w", &glm::quat::w)
		.def("__len__", &utility::constant_len_len<glm::quat, 4>)
		.def("__getitem__", &utility::constant_len_get_item<glm::quat, 4, glm::float_t>)
		.def("__setitem__", &utility::constant_len_set_item<glm::quat, 4, glm::float_t>)
		.def(self == self)
		.def(self != self)
		/*.def(self + self)
		.def(self - self)*/
		.def(self * glm::float_t())
		.def(glm::float_t() * self)
		/*.def(self += self)
		.def(self -= self)*/
		.def(self *= glm::float_t())
		.def(self /= glm::float_t());
		//.def(self_ns::str(self));
}
}} // namespace anh::utilities