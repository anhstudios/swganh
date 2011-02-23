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

#include <gtest/gtest.h>

#include <anh/component/object_manager.h>
#include <anh/component/test_components_unittest.h>

using namespace anh::component;
using namespace anh::test_components;

#define TEST_OBJECT_ID 0xDEADBABE

/// Attach a component and see if the object now has the interface in its components list.
/// Then detach it to make sure the interface is removed from its components list.
TEST(ObjectManagerTest, AttachDetachComponent) {
	ObjectManager object_manager;

	std::shared_ptr<TransformComponentInterface> component( new TransformComponent(TEST_OBJECT_ID) );
	
	object_manager.AttachComponent(TEST_OBJECT_ID, component);
	EXPECT_TRUE(object_manager.HasInterface(TEST_OBJECT_ID, ComponentType("TransformComponent")));
	
	object_manager.DetachComponent(TEST_OBJECT_ID, ComponentType("TransformComponent"));
	EXPECT_FALSE(object_manager.HasInterface(TEST_OBJECT_ID, ComponentType("TransformComponent")));
}

/// Broadcast a message to the object's components, and check for a changed value within the test component.
/// In this case, we attach a HAMComponent and broadcast a DoDamageMessage and varify the results.
TEST(ObjectManagerTest, BroadcastMessage) {
	ObjectManager object_manager;
	std::shared_ptr<HAMComponentInterface> component( new HAMComponent(TEST_OBJECT_ID) );

	component->ham().health = 1000;
	component->ham().action = 1000;
	component->ham().mind = 1000;

	object_manager.AttachComponent(TEST_OBJECT_ID, component);
	object_manager.BroadcastMessage(TEST_OBJECT_ID, Message( new DoDamageMessage(1, 500) ));
	object_manager.BroadcastMessage(TEST_OBJECT_ID, Message( new DoDamageMessage(2, 550) ));
	object_manager.BroadcastMessage(TEST_OBJECT_ID, Message( new DoDamageMessage(3, 600) ));

	EXPECT_EQ(500, component->ham().health);
	EXPECT_EQ(450, component->ham().action);
	EXPECT_EQ(400, component->ham().mind);

	object_manager.DetachComponent(TEST_OBJECT_ID, ComponentType("HAMComponent"));
}

/// Tick the ObjectManager and make sure components added to the update list are ticked as well.
TEST(ObjectManagerTest, Tick) {
	ObjectManager object_manager;
	std::shared_ptr<TickableComponentInterface> component( new TickableComponent(TEST_OBJECT_ID) );

	object_manager.AttachComponent(TEST_OBJECT_ID, component);
	EXPECT_FALSE(component->ticked());

	object_manager.Tick(0);
	EXPECT_TRUE(component->ticked());

	object_manager.DetachComponent(TEST_OBJECT_ID, ComponentType("TickableComponent"));
}

/// Attach a component and query for the interface. Check to see that a NullComponent was not returned
/// and that the correct Component type was returned with the correctly attached ObjectId.
TEST(ObjectManagerTest, QueryInterface) {
	ObjectManager object_manager;

	object_manager.AttachComponent(TEST_OBJECT_ID, std::shared_ptr<TransformComponentInterface>(new TransformComponent(TEST_OBJECT_ID)));
	std::shared_ptr<TransformComponentInterface> component = object_manager.QueryInterface<TransformComponentInterface>(TEST_OBJECT_ID, ComponentType("TransformComponent"));

	EXPECT_FALSE(ComponentType("NullTransformComponent") == component->component_info().type);
	EXPECT_TRUE(ComponentType("TransformComponent") == component->component_info().type);
	EXPECT_EQ(TEST_OBJECT_ID, component->object_id());

	object_manager.DetachComponent(TEST_OBJECT_ID, ComponentType("TransformComponent"));
}

/// When a component interface is asked for that doesnt exist, a NullComponent should be returned.
TEST(ObjectManagerTest, QueryInterfaceNullComponentReturn) {
	ObjectManager object_manager;

	std::shared_ptr<TransformComponentInterface> component = object_manager.QueryInterface<TransformComponentInterface>(TEST_OBJECT_ID, ComponentType("TransformComponent"));
	EXPECT_FALSE(ComponentType("TransformComponent") == component->component_info().type);
	EXPECT_TRUE(ComponentType("NullTransformComponent") == component->component_info().type);
}
/// test that all components detatch from an object id
TEST(ObjectManagerTest, AllObjectsDetatch) {
    ObjectManager object_manager;
    
    object_manager.AttachComponent(TEST_OBJECT_ID, std::shared_ptr<TransformComponentInterface>(new TransformComponent(TEST_OBJECT_ID)));
    object_manager.AttachComponent(TEST_OBJECT_ID, std::shared_ptr<AppearanceComponentInterface>(new AppearanceComponent(TEST_OBJECT_ID)));
    std::shared_ptr<TransformComponentInterface> transform_component = object_manager.QueryInterface<TransformComponentInterface>(TEST_OBJECT_ID, ComponentType("TransformComponent"));
    std::shared_ptr<AppearanceComponentInterface> appearance_component = object_manager.QueryInterface<AppearanceComponentInterface>(TEST_OBJECT_ID, ComponentType("Appearance"));

    EXPECT_TRUE(ComponentType("TransformComponent") == transform_component->component_info().type);
    EXPECT_TRUE(ComponentType("Appearance") == appearance_component->component_info().type);

    // detatch
    object_manager.DetatchAllComponents(TEST_OBJECT_ID);
    // after detatched we should no longer be able to grab the components with this id
    transform_component = object_manager.QueryInterface<TransformComponentInterface>(TEST_OBJECT_ID, ComponentType("TransformComponent"));
    EXPECT_TRUE(ComponentType("NullTransformComponent") == transform_component->component_info().type);
    appearance_component = object_manager.QueryInterface<AppearanceComponentInterface>(TEST_OBJECT_ID, ComponentType("Appearance"));
    EXPECT_TRUE(ComponentType("NullAppearance") == appearance_component->component_info().type);
    EXPECT_EQ(0, transform_component->object_id());
    EXPECT_EQ(0, appearance_component->object_id());
}