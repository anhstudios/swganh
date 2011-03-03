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
#ifndef ZONE_TRANSFORM_COMPONENT_H
#define ZONE_TRANSFORM_COMPONENT_H

#include <anh/component/component_interface.h>
#include <anh/component/base_component.h>
#include <glm/glm.hpp>

using namespace anh::component;
namespace zone { namespace components {

//From http://wiki.swganh.org/index.php/DataTransform_(00000071)
class TransformMessage : public SimpleMessage
{
public:
	TransformMessage(ObjectId parent, glm::vec3 position, glm::vec4 rotation, float speed)
        // 0 priority is the highest
		: SimpleMessage(MessageType("Transform"), 0)
		, parent_id_(parent)
        , position_(position)
        , rotation_(rotation)
        , speed_(speed) { }

	const ObjectId& parent_id()  { return parent_id_; }
    const glm::vec3& position()  { return position_; }
    const glm::vec4& rotation()  { return rotation_; }
    const float& speed() { return speed_; }

private:
	ObjectId parent_id_;
	glm::vec3 position_;
    glm::vec4 rotation_;
    float speed_;
};

class NullTransformComponent;

class TransformComponentInterface : public BaseComponent {
    public:
	TransformComponentInterface(const ObjectId& id)
		: BaseComponent(id) { }
    virtual ObjectId& parent_id() = 0;
    virtual glm::vec3& position() = 0;
    virtual glm::vec4& rotation() = 0;
    virtual float& speed() = 0;

    static std::shared_ptr<NullTransformComponent> NullComponent;
};

class NullTransformComponent : public TransformComponentInterface {
public:
    NullTransformComponent()
		: TransformComponentInterface(0) { }

    ObjectId& parent_id() { return parent_id_; }
	glm::vec3& position() { return position_; }
	glm::vec4& rotation() { return rotation_; }
    float& speed() { return speed_; }
    
	const ComponentInfo& component_info() { return component_info_; }
private:
    ObjectId parent_id_;
    glm::vec3 position_;
    glm::vec4 rotation_;
    float speed_;
    static ComponentInfo component_info_;
};
class TransformComponent : public TransformComponentInterface {
public:
    TransformComponent(const ObjectId& id);

	void Init(boost::property_tree::ptree& pt);
    void HandleMessage(const Message message);

    void parent_id(const ObjectId& id) { parent_id_ = id; }
    ObjectId& parent_id() { return parent_id_; }
    void position(const glm::vec3& position) { position_ = position; }
    void position(float x, float y, float z) { position_.x = x; position_.y = y; position_.z = z;}
	glm::vec3& position() { return position_; }
    void rotation(const glm::vec4& rotation) { rotation_ = rotation; }
    void rotation(float x, float y, float z, float w) { rotation_.x = x; rotation_.y = y; rotation_.z = z; rotation_.w = w;}
	glm::vec4& rotation() { return rotation_; }
    void speed(const float& speed) { speed_ = speed; }
    float& speed() { return speed_; }
    
	const ComponentInfo& component_info() { return component_info_; }
private:
    TransformComponent();
    ObjectId parent_id_;
    glm::vec3 position_;
    glm::vec4 rotation_;
    float speed_;
    static ComponentInfo component_info_;
};

} // components
} // zone
#endif //ZONE_TRANSFORM_COMPONENT_H