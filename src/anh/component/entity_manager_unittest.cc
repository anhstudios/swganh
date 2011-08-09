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

#include <anh/component/entity_manager.h>
#include <anh/component/mock_component.h>

using namespace anh::component;
using namespace testing;

class EntityManagerTests : public Test {
protected:
	virtual void SetUp();
	virtual void TearDown();

	EntityId entity_id_;
};

TEST_F(EntityManagerTests, AddRemoveEntity) {
	EntityManager entity_manager;

	// Add Entity and varify.
	entity_manager.AddEntity(std::make_shared<Entity>(entity_id_));
	EXPECT_EQ(entity_id_, entity_manager.GetEntity(entity_id_)->id());

	// Remove Entity and varify.
	entity_manager.RemoveEntity(entity_id_);
	EXPECT_TRUE(nullptr == entity_manager.GetEntity(entity_id_));
}

TEST_F(EntityManagerTests, AutoEntityAndComponentCleanupOnDestructor) {
	EntityManager entity_manager;
	std::shared_ptr<NiceMock<MockComponent>> component(new NiceMock<MockComponent>());

	// Create Expectations
	EXPECT_CALL(*component, OnDetach())
		.Times(1);

	// Add Entity and component.
	entity_manager.AddEntity(std::make_shared<Entity>(entity_id_));
	entity_manager.AttachComponent(entity_id_, component);
}

TEST_F(EntityManagerTests, CanRemoveAllEntities) {
	EntityManager entity_manager;

	// Add Entity and varify.
	entity_manager.AddEntity(std::make_shared<Entity>(entity_id_));
	EXPECT_EQ(entity_id_, entity_manager.GetEntity(entity_id_)->id());

	// Remove Entities and varify.
	entity_manager.RemoveAllEntities();
	EXPECT_TRUE(nullptr == entity_manager.GetEntity(entity_id_));
}

TEST_F(EntityManagerTests, AddRemoveTag) {
	EntityManager entity_manager;
	
	entity_manager.AddEntity(std::make_shared<Entity>(entity_id_));
	
	// Add Tag and varify.
	entity_manager.AddTag(entity_id_, "test");
	EXPECT_TRUE(entity_manager.HasTag(entity_id_, "test"));

	// Remove Tag and varify.
	entity_manager.RemoveTag(entity_id_, "test");
	EXPECT_FALSE(entity_manager.HasTag(entity_id_, "test"));

	entity_manager.RemoveEntity(entity_id_);
}

TEST_F(EntityManagerTests, AttachDetachComponent) {
	EntityManager entity_manager;
	auto component = std::make_shared<MockComponent>();

	// Setup Expectations
	EXPECT_CALL(*component, set_entity_id(entity_id_))
		.Times(1);

	EXPECT_CALL(*component, OnAttach())
		.Times(1);

	EXPECT_CALL(*component, OnDetach())
		.Times(1);

	// Add Entity.
	entity_manager.AddEntity(std::make_shared<Entity>(entity_id_));

	// Attach Component and varify.
	entity_manager.AttachComponent(entity_id_, component);
	EXPECT_TRUE(entity_manager.HasInterface(entity_id_, "Mock"));

	// Detach Component and varify.
	entity_manager.DetachComponent(entity_id_, "Mock");
	EXPECT_FALSE(entity_manager.HasInterface(entity_id_, "Mock"));

	entity_manager.RemoveEntity(entity_id_);
}

/// 
TEST_F(EntityManagerTests, BroadcastMessage) {
	EntityManager entity_manager;
	std::shared_ptr<NiceMock<MockComponent>> component(new NiceMock<MockComponent>());
	MessagePtr message(new SimpleMessage("mock_message"));

	// Setup Expectations
	EXPECT_CALL(*component, HandleMessage(message))
		.Times(1);

	// Add Entity and Attach Component
	entity_manager.AddEntity(std::make_shared<Entity>(entity_id_));
	entity_manager.AttachComponent(entity_id_, component);

	// Broadcast Message
	entity_manager.BroadcastMessage(entity_id_, message);

	// Detach
	entity_manager.DetachComponent(entity_id_, "Mock");

	entity_manager.RemoveEntity(entity_id_);
}

/// 
TEST_F(EntityManagerTests, CanUpdate) {
	EntityManager entity_manager;
	std::shared_ptr<NiceMock<MockComponent>> component(new NiceMock<MockComponent>());

	// Setup Expectations
	EXPECT_CALL(*component, Update(0))
		.Times(1);

	// Add Entity and Attach Component
	entity_manager.AddEntity(std::make_shared<Entity>(entity_id_));
	entity_manager.AttachComponent(entity_id_, component);

	// Update
	entity_manager.Update(0);

	// Detach
	entity_manager.DetachComponent(entity_id_, "Mock");

	entity_manager.RemoveEntity(entity_id_);
}

/// 
TEST_F(EntityManagerTests, CanQueryInterface) {
	EntityManager entity_manager;
	std::shared_ptr<NiceMock<MockComponent>> component(new NiceMock<MockComponent>());

	// Setup Expectations
	EXPECT_CALL(*component, entity_id())
		.WillRepeatedly(ReturnRef(entity_id_));

	// Add Entity and Attach.
	entity_manager.AddEntity(std::make_shared<Entity>(entity_id_));
	entity_manager.AttachComponent(entity_id_, component);

	// Query Interface
	EXPECT_EQ(ComponentType("Anh.Mock"), entity_manager.QueryInterface<MockComponentInterface>(entity_id_, "Mock")->component_type());
	EXPECT_EQ(entity_id_, entity_manager.QueryInterface<MockComponentInterface>(entity_id_, "Mock")->entity_id());

	// Detach and Remove Entity
	entity_manager.DetachComponent(entity_id_, "Mock");
	entity_manager.RemoveEntity(entity_id_);
}

///
TEST_F(EntityManagerTests, QueryInterfaceNullComponentReturn) {
	EntityManager entity_manager;

	// Add Entity
	entity_manager.AddEntity(std::make_shared<Entity>(entity_id_));

	// Query Interface
	EXPECT_EQ(ComponentType("NullMock"), entity_manager.QueryInterface<MockComponentInterface>(entity_id_, "Mock")->component_type());
	EXPECT_EQ(0, entity_manager.QueryInterface<MockComponentInterface>(entity_id_, "Mock")->entity_id());

}

void EntityManagerTests::SetUp() {
	entity_id_ = 0xDEADBABE;
}

void EntityManagerTests::TearDown() {
}
