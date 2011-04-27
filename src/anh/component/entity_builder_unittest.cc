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
    EntityBuilder entity_builder;

protected:
    virtual void SetUp();
    virtual void TearDown();

	EntityId entity_id_;
};

void EntityBuilderTests::SetUp() {
    
	// Create "Virtual" Template Bank
	boost::filesystem::create_directory("templates.temp");

	// Default Component

	// T21 Template
    std::ofstream of("templates.temp/t21.xml");
    of << "<entity type='mock_entity' tags='test'>"							<< std::endl;
	of << "  <component type='anh.mock' \>"									<< std::endl;
    of << "</entity>"														<< std::endl;

    of.flush();
    of.close();
}
void EntityBuilderTests::TearDown() {
    entity_builder.Deinit();
	boost::filesystem::remove_all("templates.temp");
}

/// When a new EntityBuilder is created, no entity templates should be registered.
TEST_F(EntityBuilderTests, NoTemplatesByDefault) {
	EXPECT_FALSE(entity_builder.TemplateExists("mock_template"));
}

/// When a new ObjectBuilder is initialized, all the templates in the passed in
/// directory path should be loaded and stored within the ObjectBuilder.
TEST_F(EntityBuilderTests, Init) {
	EntityBuilder entity_builder(std::make_shared<EntityManager>());
	entity_builder.Init("templates.temp");
}

/// A newly registered creator should return true.
TEST_F(EntityBuilderTests, CanRegisterAndUnregisterCreator) {
}

/// verifies that Loader Exists after registration and does not exist after unregister
TEST_F(EntityBuilderTests, CanRegisterAndUnregisterLoader) {
}

/// We shouldn't be able to register two creators for a single component type.
TEST_F(EntityBuilderTests, CannotRegisterTwoCreators) {
}

/// We shouldn't be able to register two loaders for a single component type.
TEST_F(EntityBuilderTests, CannotRegisterTwoLoaders) {
}

/// Test to make sure we are not able to construct an object that doesnt have a
/// template.
TEST_F(EntityBuilderTests, CannotBuildObjectWithoutTemplate) {
}

/// Make sure that if there are no component registrations whatsoever we fail to build the object
TEST_F(EntityBuilderTests, BuildWithMissingComponentRegistration) {
}

/// verifies we can still build an object and use it, if there is no loader attached
TEST_F(EntityBuilderTests, BuildSingleComponentObjectNoLoader) {
}

// verify that we can build a component object with multiple components even with no loaders
TEST_F(EntityBuilderTests, BuildMultiComponentObjectNoLoaders) {
}

// a single component object built with a loder, should build and validate the loader exists.
TEST_F(EntityBuilderTests, BuildSingleComponentObjectWithLoader) {
}

// a multiple component object built with multiple loders, should build and validate the loaders exists.
TEST_F(EntityBuilderTests, BuildMultiComponentObjectWithLoaders) {
}

//
TEST_F(EntityBuilderTests, BuildMultiComponentObjectWithMixedLoaders) {
}