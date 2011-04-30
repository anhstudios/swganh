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

#include <fstream>
#include <gtest/gtest.h>

#include <anh/component/entity_builder.h>
#include <anh/component/mock_component.h>

using namespace anh::component;

class EntityBuilderTests : public testing::Test
{
public:
	std::shared_ptr<EntityManager> entity_manager;
	std::shared_ptr<EntityBuilder> entity_builder;

protected:
    virtual void SetUp();
    virtual void TearDown();

	EntityId entity_id_;
};

void EntityBuilderTests::SetUp() {
    entity_manager = std::make_shared<EntityManager>();
    entity_builder = std::make_shared<EntityBuilder>(entity_manager);
	// Create "Virtual" Template Bank
	boost::filesystem::create_directory("templates.temp");

	// Default Component

	// T21 Template
    std::ofstream of("templates.temp/t21.xml");
    of << "<entity type='mock_entity' tags='test'>"							<< std::endl;
	of << "  <component type='anh.mock'\/>"									<< std::endl;
    of << "</entity>"														<< std::endl;

    of.flush();
    of.close();
}
void EntityBuilderTests::TearDown() {
    entity_builder->Deinit();
	boost::filesystem::remove_all("templates.temp");
}

/// When a new EntityBuilder is created, no entity templates should be registered.
TEST_F(EntityBuilderTests, NoTemplatesByDefault) {
	EXPECT_FALSE(entity_builder->TemplateExists("mock_template"));
}

/// When a new EntityBuilder is initialized, all the templates in the passed in
/// directory path should be loaded and stored within the EntityBuilder.
TEST_F(EntityBuilderTests, Init) {
	entity_builder->Init("templates.temp");
    EXPECT_TRUE(entity_builder->TemplateExists("mock_entity"));
}

/// A newly registered creator should return true.
TEST_F(EntityBuilderTests, CanRegisterAndUnregisterCreator) {
    entity_builder->RegisterCreator("Mock", [=](const EntityId& id){ return std::make_shared<MockComponent>(); });
    EXPECT_TRUE(entity_builder->CreatorExists("Mock"));
}
//
///// verifies that a db Mapper Exists after registration and does not exist after unregister
TEST_F(EntityBuilderTests, CanRegisterAndUnregisterLoader) {
    entity_builder->RegisterDBMapper("Mock", std::make_shared<MockAttributeMapper>());
    EXPECT_TRUE(entity_builder->DBMapperExists("Mock"));
    entity_builder->UnregisterDBMapper("Mock");
    EXPECT_FALSE(entity_builder->DBMapperExists("Mock"));
}

///// We shouldn't be able to register two creators for a single component type.
TEST_F(EntityBuilderTests, CannotRegisterTwoCreators) {
    EXPECT_TRUE(entity_builder->RegisterCreator("Mock", [=](const EntityId& id){ return std::make_shared<MockComponent>(); }));
    EXPECT_FALSE(entity_builder->RegisterCreator("Mock", [=](const EntityId& id){ return std::make_shared<MockComponent>(); }));
}
//
///// We shouldn't be able to register two loaders for a single component type.
TEST_F(EntityBuilderTests, CannotRegisterTwoLoaders) {
    EXPECT_TRUE(entity_builder->RegisterDBMapper("Mock", std::make_shared<MockAttributeMapper>()));
    EXPECT_FALSE(entity_builder->RegisterDBMapper("Mock", std::make_shared<MockAttributeMapper>()));
}
//
///// Test to make sure we are not able to construct an entity that doesnt have a
///// template.
TEST_F(EntityBuilderTests, CannotBuildEntityWithoutTemplate) {
    EXPECT_EQ(entity_builder->BuildEntity(entity_id_, "Mock", "test.Mock"), BUILD_FAILED);
}
//
///// Make sure that if there are no component registrations whatsoever we fail to build the entity
TEST_F(EntityBuilderTests, BuildWithMissingComponentRegistration) {
    entity_builder->Init("templates.temp");
    EXPECT_EQ(entity_builder->BuildEntity(entity_id_, "Mock", "test.Mock"), BUILD_FAILED);
}
//
///// verifies we can still build an entity and use it, if there is no DBMapper attached
TEST_F(EntityBuilderTests, BuildSingleComponentEntityNoDBMapper) {
    entity_builder->Init("templates.temp");
    EXPECT_TRUE(entity_builder->RegisterCreator("Mock", [=](const EntityId& id){ return std::make_shared<MockComponent>(); }));
    EXPECT_EQ(entity_builder->BuildEntity(entity_id_, "mock_entity", "test.Mock"), BUILD_INCOMPLETE);
    std::shared_ptr<MockComponentInterface> component = 
        entity_manager->QueryInterface<MockComponentInterface>(entity_id_, "Mock");
    EXPECT_EQ(component->component_type(), "NullMock");
}
//
//// a single component object built with a loder, should build and validate the loader exists.
TEST_F(EntityBuilderTests, BuildSingleComponentEntityWithLoader) {
    entity_builder->Init("templates.temp");
    EXPECT_TRUE(entity_builder->RegisterCreator("anh.mock", [=](const EntityId& id){ return std::make_shared<::testing::NiceMock<MockComponent>>(); }));
    EXPECT_TRUE(entity_builder->RegisterDBMapper("anh.mock", std::make_shared<::testing::NiceMock<MockAttributeMapper>>()));
    EXPECT_TRUE(entity_builder->CreatorExists("anh.mock"));
    EXPECT_TRUE(entity_builder->DBMapperExists("anh.mock"));
    EXPECT_EQ(entity_builder->BuildEntity(entity_id_, "mock_entity", "test.Mock"), BUILD_SUCCESSFUL);
    std::shared_ptr<MockComponentInterface> component = 
        entity_manager->QueryInterface<MockComponentInterface>(entity_id_, "Mock");
    EXPECT_EQ(component->component_type(), "NullMock");
}