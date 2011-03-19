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
#include "transform_component.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


using namespace std;
namespace zone { namespace components {

// statics
std::shared_ptr<NullTransformComponent> TransformComponentInterface::NullComponent = std::shared_ptr<NullTransformComponent>(new NullTransformComponent());
ComponentInfo NullTransformComponent::component_info_ = ComponentInfo(ComponentType("NullTransformComponent"), false);
ComponentInfo TransformComponent::component_info_ = ComponentInfo(ComponentType("TransformComponent"), true);

TransformComponent::TransformComponent(const ObjectId& id)
: TransformComponentInterface(id)
{}
TransformComponent::TransformComponent(const ObjectId& id, const glm::vec3& position, const glm::quat& rotation, const float speed)
    : TransformComponentInterface(id)
    , position_(position)
    , rotation_(rotation)
    , speed_(speed)
{}
void TransformComponent::Init(boost::property_tree::ptree& pt) {
    // initial default values
    parent_id_ = pt.get<ObjectId>("parent_id", 0); 
	position_.x = pt.get<float>("position.x", 0.0f);
	position_.y = pt.get<float>("position.y", 0.0f);
	position_.z = pt.get<float>("position.z", 0.0f);
	rotation_.x = pt.get<float>("rotation.x", 0.0f);
	rotation_.y = pt.get<float>("rotation.y", 0.0f);
	rotation_.z = pt.get<float>("rotation.z", 0.0f);
	rotation_.w = pt.get<float>("rotation.w", 1.0f);

}

void TransformComponent::HandleMessage(const shared_ptr<TransformMessage> message) {
    //std::shared_ptr<TransformMessage> transform = std::dynamic_pointer_cast<TransformMessage>(message);
    
    parent_id(message->parent_id());
    position(message->position());
    rotation(message->rotation());
    speed(message->speed());
}
void TransformComponent::rotate(const float& degrees) {
    // Rotate the item left by the specified degrees
    rotation_ = glm::rotate(rotation_, degrees, glm::vec3(0.0f, 1.0f, 0.0f));
}
void TransformComponent::rotate_left(const float& degrees) {
    rotate(-degrees);
}
void TransformComponent::rotate_right(const float& degrees) {
    rotate(degrees);
}
void TransformComponent::face(const glm::vec3& target_position) {
    // Create a mirror direction vector for the direction we want to face.
    glm::vec3 direction_vector = glm::normalize(target_position - position_);
    direction_vector.x = -direction_vector.x;

    // Create a lookat matrix from the direction vector and convert it to a quaternion.
    rotation_ = glm::toQuat(glm::lookAt(
                                 direction_vector, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
                             ));

    // If in the 3rd quadrant the signs need to be flipped.
    if (rotation_.y <= 0.0f && rotation_.w >= 0.0f) {
        rotation_.y = -rotation_.y;
        rotation_.w = -rotation_.w;
    }
}
void TransformComponent::move(const glm::quat& rotation, float distance) {
    // Create a vector of the length we want pointing down the x-axis.
    glm::vec3 movement_vector(0.0f, 0.0f, distance);

    // Rotate the movement vector by the direction it should be facing.
    movement_vector = rotation_ * movement_vector;

    // Add the movement vector to the current position to get the new position.
    position_ += movement_vector;
}
void TransformComponent::move_forward(const float& distance) {
    move(rotation_, distance);
}
void TransformComponent::move_back(const float& distance) {
    move(rotation_, -distance);
}
float TransformComponent::rotation_angle() const {
    glm::quat tmp = rotation_;

    if (tmp.y < 0.0f && tmp.w > 0.0f) {
        tmp.y *= -1;
        tmp.w *= -1;
    }

    return glm::angle(tmp);
}

} // namespace zone
} // namespace components