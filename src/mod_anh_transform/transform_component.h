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
#ifndef MOD_ANH_TRANSFORM_TRANFORM_COMPONENT_H
#define MOD_ANH_TRANSFORM_TRANFORM_COMPONENT_H

#include <api/components/transform_component_interface.h>
#include <mod_anh_transform/transform_db_mapper.h>
#include <boost/flyweight.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <anh_definitions.h>

using namespace anh::component;
namespace anh_api = anh::api::components;

namespace transform {

//From http://wiki.swganh.org/index.php/DataTransform_(00000071)
class TransformMessage : public SimpleMessage
{
public:
	TransformMessage(EntityId parent, glm::vec3 position, glm::quat rotation, float speed)
        // 0 priority is the highest
		: SimpleMessage(MessageType("Transform"), 0)
		, parent_id_(parent)
        , position_(position)
        , rotation_(rotation)
        , speed_(speed) { }

    static std::shared_ptr<TransformMessage> createTransformMessage(EntityId id, glm::vec3 pos, glm::quat rot, float speed) {
        return std::make_shared<TransformMessage>(id, pos, rot, speed); 
    }

	const EntityId parent_id()  { return parent_id_; }
    const glm::vec3& position()  { return position_; }
    const glm::quat& rotation()  { return rotation_; }
    const float speed() { return speed_; }

private:
	EntityId parent_id_;
	glm::vec3 position_;
    glm::quat rotation_;
    float speed_;
};
class DLL_EXPORT TransformComponent : public anh_api::TransformComponentInterface {
public:
    TransformComponent();
    TransformComponent(const glm::vec3& position, const glm::quat& rotation, const float speed);

	void Init(boost::property_tree::ptree& pt);
	virtual void Update(const float deltaMilliseconds);
    virtual void HandleMessage(const std::shared_ptr<TransformMessage> message);

    void parent_id(const EntityId id) { parent_id_ = id; }
    const EntityId parent_id() { return parent_id_; }
    void position(const glm::vec3& position) { position_ = position; }
    void position(const float x, const float y, const float z) { position_.x = x; position_.y = y; position_.z = z;}
	const glm::vec3& position() { return position_; }
    void rotation(const glm::quat& rotation) { rotation_ = rotation; }
    void rotation(const float x, const float y, const float z, const float w) { rotation_.x = x; rotation_.y = y; rotation_.z = z; rotation_.w = w;}
	const glm::quat& rotation() { return rotation_; }
    void speed(const float speed) { speed_ = speed; }
    const float speed() { return speed_; }

    // convenience commands
    // rotate by degrees
    void rotate(const float& degrees);
    void rotate_left(const float& degrees);
    void rotate_right(const float& degrees);
    void face(const glm::vec3& target_position);
    void move(const glm::quat& rotation, float distance);
    void move_forward(const float& distance);
    void move_back(const float& distance);
    float rotation_angle() const;

private:
    boost::flyweight<EntityId> parent_id_;
    glm::vec3 position_;
    glm::quat rotation_;
    boost::flyweight<float> speed_;
};

} // transform
#endif //MOD_ANH_TRANSFORM_TRANFORM_COMPONENT_H