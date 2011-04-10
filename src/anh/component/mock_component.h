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

#ifndef ANH_COMPONENT_MOCK_COMPONENT_H_
#define ANH_COMPONENT_MCOK_COMPONENT_H_

#include <gmock/gmock.h>
#include <anh/component/component_interface.h>

// Generate mock Component.

namespace anh {
namespace component {

class NullMockComponent;

class MockComponent : public ComponentInterface {
public:
	MOCK_METHOD1(Init, void (boost::property_tree::ptree& pt));
	MOCK_METHOD0(Deinit, void ());
	MOCK_METHOD1(Update, void (const float timeout));
	MOCK_METHOD1(HandleMessage, void (const Message message));
	MOCK_METHOD0(component_info, const ComponentInfo& ());
	MOCK_METHOD0(object_id, const ObjectId& ());

	static std::shared_ptr<MockComponent> NullComponent;
};

} // namespace component
} // namespace anh

#endif // ANH_COMPONENT_MOCK_COMPONENT_H_