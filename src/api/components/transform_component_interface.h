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
#ifndef ANH_API_COMPONENTS_TRANSFORM_COMPONENT_INTERFACE_H
#define ANH_API_COMPONENTS_TRANSFORM_COMPONENT_INTERFACE_H

#include <anh/component/component_interface.h>
#include <anh/component/base_component.h>

#include <boost/flyweight.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>

using namespace anh::component;
namespace anh {namespace api { namespace components {

class NullTransformComponent;

class TransformComponentInterface : public BaseComponent {
    public:
	TransformComponentInterface(const ComponentType& type)
		: BaseComponent("Transform", type) { }

    virtual const EntityId parent_id() = 0;
    virtual const glm::vec3& position() = 0;
    virtual void position(const float x, const float y, const float z) = 0;
    virtual const glm::quat& rotation() = 0;
    virtual void rotation(const float x, const float y, const float z, const float w) = 0;
    virtual const float speed() = 0;

    // convenience commands
    virtual void rotate(const float& degrees) = 0;
    virtual void rotate_left(const float& degrees) = 0;
    virtual void rotate_right(const float& degrees) = 0;
    virtual void face(const glm::vec3& target_position) = 0;
    virtual void move(const glm::quat& rotation, float distance) = 0;
    virtual void move_forward(const float& distance) = 0;
    virtual void move_back(const float& distance) = 0;
    virtual float rotation_angle() const = 0;

    static std::shared_ptr<NullTransformComponent> NullComponent;
};
class NullTransformComponent : public TransformComponentInterface {
public:
    NullTransformComponent()
		: TransformComponentInterface("NullTransform") { }
	
    const EntityId parent_id() { return parent_id_; }
    virtual const glm::vec3& position() { return position_; }
    void position(const float x, const float y, const float z){}
    const glm::quat& rotation() { return rotation_; }
    void rotation(const float x, const float y, const float z, const float w){};
    void rotate(const float& degrees){};
    void rotate_left(const float& degrees){};
    void rotate_right(const float& degrees){};
    void face(const glm::vec3& target_position){};
    void move(const glm::quat& rotation, float distance){};
    void move_forward(const float& distance){};
    void move_back(const float& distance){};
    float rotation_angle() const{ return 0.0f; }
    const float speed() { return speed_; }
private:
    boost::flyweight<EntityId> parent_id_;
    glm::vec3 position_;
    glm::quat rotation_;
    boost::flyweight<float> speed_;
};
} // components
} // api
} // anh
#endif //ANH_API_COMPONENTS_TRANSFORM_COMPONENT_INTERFACE_H