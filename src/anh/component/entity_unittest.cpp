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
#include <anh/component/test_components_unittest.h>
#include <memory.h>

using namespace anh::component;
using namespace anh::test_components;
using namespace testing;

std::shared_ptr<MockComponent> MockComponent::NullComponent;

/// Test fixture for testing the Entity class, provides setup/teardown.
class EntityTest : public testing::Test {
protected:
	virtual void SetUp();
	virtual void TearDown();

	ObjectId entity_id_;
};

/// By default, no tags should be attached to the entity if no TagSet is passed
/// in through the constructor.
TEST_F(EntityTest, NoTagsByDefault)
{
	Entity entity;
	EXPECT_FALSE(entity.hasTag("test.tag"));
}

/// Tests the ability to add and remove (as well as validate the existance of) tags
/// on an entity.
TEST_F(EntityTest, CanAddRemoveTags)
{
	Entity entity;

	entity.addTag("test.tag");
	EXPECT_TRUE(entity.hasTag("test.tag"));

	entity.removeTag("test.tag");
	EXPECT_FALSE(entity.hasTag("test.tag"));
}

/// By default, no components should be attached to our entity.
TEST_F(EntityTest, NoComponentsAttachedByDefault)
{
	Entity entity;
	EXPECT_FALSE(entity.hasComponent("mock_component"));
}

/// Tests the ability to add and remove ( as well as validate the existance of) components
/// on an entity.
TEST_F(EntityTest, CanAttachDetachComponent)
{
	Entity entity(entity_id_);
	std::shared_ptr<TransformComponentInterface> transform_component(new TransformComponent(entity_id_));

	entity.AttachComponent(transform_component);
	EXPECT_TRUE(entity.hasComponent("TransformComponent"));

	entity.DetachComponent("TransformComponent");
	EXPECT_FALSE(entity.hasComponent("TransformComponent"));
}

///
TEST_F(EntityTest, CanBroadcastMessage)
{
	Entity entity(entity_id_);
	std::shared_ptr<HAMComponentInterface> ham_component(new HAMComponent(entity_id_));

	ham_component->ham().health = 1000;
	ham_component->ham().action = 1000;
	ham_component->ham().mind = 1000;

	entity.AttachComponent(ham_component);
	entity.BroadcastMessage(anh::component::Message(new DoDamageMessage(1, 10)));
	entity.BroadcastMessage(anh::component::Message(new DoDamageMessage(2, 20)));
	entity.BroadcastMessage(anh::component::Message(new DoDamageMessage(3, 30)));

	EXPECT_EQ(990, ham_component->ham().health);
	EXPECT_EQ(980, ham_component->ham().action);
	EXPECT_EQ(970, ham_component->ham().mind);

	entity.DetachAllComponents();
}

///
TEST_F(EntityTest, CanQueryInterface)
{
	/*
	Entity entity(100);
	MockComponent mock_component;
	ObjectId id = 100;
	ComponentType mock_component_type_("mock_component");

	ON_CALL(mock_component, component_info())
		.WillByDefault(ReturnRef(mock_component_type_));

	EXPECT_EQ( mock_component_type_.ident(), mock_component.component_info().type.ident() ) << "mock_component_type.ident() = " << mock_component_type_.ident() << " ComponentType.ident() = " << mock_component.component_info().type.ident();
	
	//ON_CALL(mock_component, object_id())
	//	.WillByDefault(ReturnRef(id));

	//EXPECT_EQ(100, mock_component.object_id());

	//entity.AttachComponent(std::shared_ptr<ComponentInterface>(&mock_component));
	//EXPECT_EQ(id_, entity.QueryInterface<MockComponent>("mock_component")->object_id());
	//entity.DetachAllComponents();
	*/

	Entity entity(entity_id_);
	std::shared_ptr<TransformComponentInterface> transform_component(new TransformComponent(entity_id_));

	// Attach
	entity.AttachComponent(transform_component);
	EXPECT_TRUE(entity.hasComponent("TransformComponent"));

	// Check to make sure the components information matches and that this is infact, that same instance.
	EXPECT_EQ(entity_id_, entity.QueryInterface<TransformComponentInterface>("TransformComponent")->object_id());
	EXPECT_EQ(ComponentType("TransformComponent"), entity.QueryInterface<TransformComponentInterface>("TransformComponent")->component_info().type);

	// Detach
	entity.DetachComponent("TransformComponent");
	EXPECT_FALSE(entity.hasComponent("TransformComponent"));
}

///
TEST_F(EntityTest, CanUpdate)
{
	Entity entity(entity_id_);
	std::shared_ptr<TickableComponent> tickable_component(new TickableComponent(entity_id_));

	// Attach
	entity.AttachComponent(tickable_component);
	EXPECT_TRUE(entity.hasComponent("TickableComponent"));

	entity.Update(0);
	EXPECT_TRUE(tickable_component->ticked());

	// Detach
	entity.DetachAllComponents();
	EXPECT_FALSE(entity.hasComponent("TickableComponent"));
}

/*****************************************************************************/
// Implementation for the test fixture //

void EntityTest::SetUp(void)
{
	//MockComponent::NullComponent = std::shared_ptr<MockComponent>(new MockComponent());

	//ON_CALL(*MockComponent::NullComponent, component_info())
	//	.WillByDefault(ReturnRef(ComponentType("null_mock_component")));

	entity_id_ = 0xDEADBABE;

}

void EntityTest::TearDown(void)
{
	//delete MockComponent::NullComponent.get();
}