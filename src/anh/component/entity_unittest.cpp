/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

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

#include <anh/component/entity.h>
#include <anh/component/mock_component.h>

using namespace anh::component;
using namespace testing;

std::shared_ptr<NullMockComponent> MockComponentInterface::NullComponent(new NullMockComponent);

/// Test fixture for testing the Entity class, provides setup/teardown.
class EntityTests : public testing::Test {
protected:
	virtual void SetUp();
	virtual void TearDown();

	EntityId entity_id_;
};

/// By default, no tags should be attached to the entity if no TagSet is passed
/// in through the constructor.
TEST_F(EntityTests, NoTagsByDefault)
{
	Entity entity;
	EXPECT_FALSE(entity.HasTag("test"));
}

/// Tests the ability to add and remove (as well as validate the existance of) tags
/// on an entity.
TEST_F(EntityTests, CanAddRemoveTags)
{
	Entity entity;

	entity.AddTag("test");
	EXPECT_TRUE(entity.HasTag("test"));

	entity.RemoveTag("test");
	EXPECT_FALSE(entity.HasTag("test"));
}

/// By default, no components should be attached to our entity.
TEST_F(EntityTests, NoComponentsAttachedByDefault)
{
	Entity entity;
	EXPECT_FALSE(entity.HasInterface("Mock"));
}

/// Tests the ability to add and remove (as well as validate the existance of) components
/// an entity holds.
TEST_F(EntityTests, CanAttachDetachComponent)
{
	Entity entity(entity_id_);
	std::shared_ptr<MockComponent> component(new MockComponent());

	// Setup Expectations
	EXPECT_CALL(*component, set_entity_id(entity_id_))
		.Times(1);

	EXPECT_CALL(*component, OnAttach())
		.Times(1);

	EXPECT_CALL(*component, OnDetach())
		.Times(1);

	// Attach
	entity.AttachComponent(component);
	EXPECT_TRUE(entity.HasInterface("Mock"));

	// Detach
	entity.DetachComponent("Mock");
	EXPECT_FALSE(entity.HasInterface("Mock"));
}

/// Varifies the HandleMessage function is called on each component when the BroadcastMessage
/// function is called.
TEST_F(EntityTests, CanBroadcastMessage)
{
	Entity entity(entity_id_);
	std::shared_ptr<NiceMock<MockComponent>> component(new NiceMock<MockComponent>());
	MessagePtr message(new SimpleMessage("mock_message"));

	// Setup Expectations
	EXPECT_CALL(*component, HandleMessage(message))
		.Times(1);

	// Attach
	entity.AttachComponent(component);

	// Broadcast
	entity.BroadcastMessage(message);

	// Detach
	entity.DetachComponent("Mock");
}

/// Attaches a mock component to the test Entity, then varifies that the
/// QueryInterface successfully returns the same component when called.
TEST_F(EntityTests, CanQueryInterface)
{
	Entity entity(entity_id_);
	std::shared_ptr<NiceMock<MockComponent>> component(new NiceMock<MockComponent>());

	// Setup Expectations
	EXPECT_CALL(*component, entity_id())
		.WillRepeatedly(ReturnRef(entity_id_));

	// Attach
	entity.AttachComponent(component);

	// Query Interface
	EXPECT_EQ(ComponentType("Anh.Mock"), entity.QueryInterface<MockComponentInterface>("Mock")->component_type());
	EXPECT_EQ(entity_id_, entity.QueryInterface<MockComponentInterface>("Mock")->entity_id());

	// Detach
	entity.DetachComponent("Mock");
}

/// Varifies that a NullComponent will be returned if there is no concrete
/// implimentation bound to the interface.
TEST_F(EntityTests, CanQueryInterfaceNullReturn)
{
	Entity entity(entity_id_);
	
	// Query Interface
	EXPECT_EQ(ComponentType("NullMock"), entity.QueryInterface<MockComponentInterface>("Mock")->component_type());
	EXPECT_EQ(0, entity.QueryInterface<MockComponentInterface>("Mock")->entity_id());
}

/// Varifies that the Update function is called for all componeonts
/// attached to an entity when the entity Update function is called.
TEST_F(EntityTests, CanUpdate)
{
	Entity entity(entity_id_);
	std::shared_ptr<NiceMock<MockComponent>> component(new NiceMock<MockComponent>());

	// Setup Expectations
	EXPECT_CALL(*component, Update(0.0f))
		.Times(1);

	// Attach
	entity.AttachComponent(component);

	// Update
	entity.Update(0.0f);

	// Detach
	entity.DetachComponent("Mock");
}
// verifies the entity calls the Persist function on a DB Mapper on update, if dirty
TEST_F(EntityTests, CallsDBMapperOnUpdateIfDirty)
{
	Entity entity(entity_id_);
	std::shared_ptr<NiceMock<MockComponent>> component(new NiceMock<MockComponent>());

	// Setup Expectations
	EXPECT_CALL(*component, Update(0.0f))
		.Times(1);

	// Attach
	entity.AttachComponent(component);

    EXPECT_CALL(*component, dirty())
        .WillOnce(Return(true));

    component->set_dirty(true);

    EXPECT_CALL(*component, db_mapper())
        .Times(1)
        .WillRepeatedly(Return(std::shared_ptr<anh::component::AttributeMapperInterface<ComponentInterface>>()));

	// Update
	entity.Update(0.0f);

	// Detach
	entity.DetachComponent("Mock");
}
// verifies the entity wont call the Persist function on a DB Mapper on update, if NOT dirty
TEST_F(EntityTests, DoesntCallsDBMapperOnUpdateIfNotDirty)
{
	Entity entity(entity_id_);
	std::shared_ptr<NiceMock<MockComponent>> component(new NiceMock<MockComponent>());

	// Setup Expectations
	EXPECT_CALL(*component, Update(0.0f))
		.Times(1);

	// Attach
	entity.AttachComponent(component);

    EXPECT_CALL(*component, dirty())
        .WillOnce(Return(false));
    // ensure it doesn't get called
    EXPECT_CALL(*component, db_mapper())
        .Times(0);

	// Update
	entity.Update(0.0f);

	// Detach
	entity.DetachComponent("Mock");
}

/*****************************************************************************/
// Implementation for the test fixture //

void EntityTests::SetUp(void)
{
	entity_id_ = 0xDEADBABE;
}

void EntityTests::TearDown(void)
{
}