// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

namespace swganh
{
namespace tre
{
	class PobVisitor;
	class FlrVisitor;
}

namespace object
{

struct CollisionData
{	
	//Bounding Box


	//Floor Collision
	std::shared_ptr<tre::FlrVisitor> floor_data_;

	//Portal Object
	std::shared_ptr<tre::PobVisitor> portal_data_;
};

}}