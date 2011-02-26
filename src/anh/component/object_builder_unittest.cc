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
#include <boost/filesystem.hpp>
#include <anh/component/object_manager.h>
#include <anh/component/object_builder.h>
#include <anh/component/test_components_unittest.h>

using namespace std;
using namespace boost::filesystem;
using namespace anh::component;
using namespace anh::test_components;


#define TEST_OBJECT_ID 0xDEADBABE

class ObjectBuilderTest : public testing::Test
{
public:
    ObjectBuilder object_builder;
protected:
    virtual void SetUp();
    virtual void TearDown();
};

void ObjectBuilderTest::SetUp() {
    // create test files
    create_directory("templates");
    ofstream of("templates/t21.xml");
    of << "<object name=\"t21\">"<<endl;
        of << "<component type=\"TransformComponent\" />" <<endl;
        of << "<componnet type=\"Radial\">" <<endl;
         of << "<radials>" << endl;
            of << "<radial parent=\"0\" index=\"0\">Examine</radial>" <<endl;
            of << "<radial parent=\"0\" index=\"1\">Equip</radial>" <<endl;
         of << "</radials>" <<endl;
        of << "</componnet>" <<endl;
        of << "<component type=\"Appearance\">" <<endl;
        of << "<iff></iff>" <<endl;
        of << "<crc></crc>" <<endl;
        of << "</component>" <<endl;
    of << "</object>" <<endl;
    of.flush();
    of.close();
    of.open("templates/ion_rifle.xml");
    of << "<object name=\"ion_rifle\"><component name=\"TransformComponent\" /></object>" <<endl;
    of.flush();
    of.close();
}
void ObjectBuilderTest::TearDown() {
    object_builder.Deinit();
    remove_all("templates");
}

/// When a new ObjectBuilder instance is instatiated, no templates should exist.
TEST_F(ObjectBuilderTest, NoTemplatesByDefault) {
	EXPECT_FALSE(object_builder.TemplateExists("t21"));
}

/// When a new ObjectBuilder is initialized, all the templates in the passed in
/// directory path should be loaded and stored within the ObjectBuilder.
TEST_F(ObjectBuilderTest, Init) {
	object_builder.Init("templates");

	EXPECT_TRUE(object_builder.TemplateExists("t21"));
	EXPECT_TRUE(object_builder.TemplateExists("ion_rifle"));
}

/// A newly registered creator should return true.
TEST_F(ObjectBuilderTest, CanRegisterAndUnregisterCreator) {
	object_builder.RegisterCreator("TransformComponent", [=](const ObjectId& id){ return shared_ptr<ComponentInterface>( new TransformComponent(id) ); });
	EXPECT_TRUE(object_builder.CreatorExists("TransformComponent"));

	object_builder.UnregisterCreator("TransformComponent");
	EXPECT_FALSE(object_builder.CreatorExists("TransformComponent"));
}

/// verifies that Loader Exists after registration and does not exist after unregister
TEST_F(ObjectBuilderTest, CanRegisterAndUnregisterLoader) {
	object_builder.RegisterLoader("TransformComponent", shared_ptr<ComponentLoaderInterface>( new TransformComponentLoader() ));
	EXPECT_TRUE(object_builder.LoaderExists("TransformComponent"));

	object_builder.UnregisterLoader("TransformComponent");
	EXPECT_FALSE(object_builder.LoaderExists("TransformComponent"));
}

/// We shouldn't be able to register two creators for a single component type.
TEST_F(ObjectBuilderTest, CannotRegisterTwoCreators) {
	EXPECT_TRUE(object_builder.RegisterCreator("TransformComponent", [=](const ObjectId& id){ return shared_ptr<ComponentInterface>( new TransformComponent(id) ); }));
	EXPECT_FALSE(object_builder.RegisterCreator("TransformComponent", [=](const ObjectId& id){ return shared_ptr<ComponentInterface>( new TransformComponent(id) ); }));
}

/// We shouldn't be able to register two loaders for a single component type.
TEST_F(ObjectBuilderTest, CannotRegisterTwoLoaders) {
	shared_ptr<ComponentLoaderInterface> loader( new TransformComponentLoader() );
	EXPECT_TRUE(object_builder.RegisterLoader("TransformComponent", loader));
	EXPECT_FALSE(object_builder.RegisterLoader("TransformComponent", loader));
    EXPECT_TRUE(object_builder.LoaderExists("TransformComponent"));
}

/// Test to make sure we are not able to construct an object that doesnt have a
/// template.
TEST_F(ObjectBuilderTest, CannotBuildObjectWithoutTemplate) {
	EXPECT_EQ(object_builder.BuildObject(TEST_OBJECT_ID, "t21"), BUILD_FAILED);
}

/// Make sure that if there are no component registrations whatsoever we fail to build the object
TEST_F(ObjectBuilderTest, BuildWithMissingComponentRegistration) {
	object_builder.Init("templates");	
    EXPECT_EQ(object_builder.BuildObject(TEST_OBJECT_ID, "t21"), BUILD_FAILED);
}

/// verifies we can still build an object and use it, if there is no loader attached
TEST_F(ObjectBuilderTest, BuildSingleComponentObjectNoLoader) {
	object_builder.Init("templates");

	object_builder.RegisterCreator("TransformComponent", [=](const ObjectId& id){ return shared_ptr<ComponentInterface>( new TransformComponent(id) ); });
    EXPECT_EQ(object_builder.BuildObject(TEST_OBJECT_ID, "t21"), BUILD_INCOMPLETE);

	shared_ptr<TransformComponentInterface> component = gObjectManager.QueryInterface<TransformComponentInterface>(TEST_OBJECT_ID, "TransformComponent");
	
	EXPECT_EQ(TEST_OBJECT_ID, component->object_id());
	EXPECT_TRUE(component->component_info().type == ComponentType("TransformComponent"));
}

// verify that we can build a component object with multiple components even with no loaders
TEST_F(ObjectBuilderTest, BuildMultiComponentObjectNoLoaders) {
    object_builder.Init("templates");

	object_builder.RegisterCreator("TransformComponent", [=](const ObjectId& id){ return shared_ptr<ComponentInterface>( new TransformComponent(id) ); });
    object_builder.RegisterCreator("Appearance", [=] (const ObjectId& id) { return shared_ptr<ComponentInterface>( new AppearanceComponent(id) ); });
    EXPECT_EQ(object_builder.BuildObject(TEST_OBJECT_ID, "t21"), BUILD_SUCCESSFUL);

	shared_ptr<TransformComponentInterface> component = gObjectManager.QueryInterface<TransformComponentInterface>(TEST_OBJECT_ID, "TransformComponent");
	
	EXPECT_EQ(TEST_OBJECT_ID, component->object_id());
	EXPECT_TRUE(component->component_info().type == ComponentType("TransformComponent"));
}

// a single component object built with a loder, should build and validate the loader exists.
TEST_F(ObjectBuilderTest, BuildSingleComponentObjectWithLoader) {
    object_builder.Init("templates");

	object_builder.RegisterCreator("TransformComponent", [=](const ObjectId& id){ return shared_ptr<ComponentInterface>( new TransformComponent(id) ); });
    shared_ptr<ComponentLoaderInterface> loader( new TransformComponentLoader() );
	EXPECT_TRUE(object_builder.RegisterLoader("TransformComponent", loader));
    EXPECT_EQ(object_builder.BuildObject(TEST_OBJECT_ID, "t21"), BUILD_INCOMPLETE);
    EXPECT_TRUE(object_builder.LoaderExists("TransformComponent"));

	shared_ptr<TransformComponentInterface> component = gObjectManager.QueryInterface<TransformComponentInterface>(TEST_OBJECT_ID, "TransformComponent");
	
	EXPECT_EQ(TEST_OBJECT_ID, component->object_id());
	EXPECT_TRUE(component->component_info().type == ComponentType("TransformComponent"));
}

// a multiple component object built with multiple loders, should build and validate the loaders exists.
TEST_F(ObjectBuilderTest, BuildMultiComponentObjectWithLoaders) {
    object_builder.Init("templates");

	object_builder.RegisterCreator("TransformComponent", [=](const ObjectId& id){ return shared_ptr<ComponentInterface>( new TransformComponent(id) ); });
    shared_ptr<ComponentLoaderInterface> loader( new TransformComponentLoader() );
    shared_ptr<ComponentLoaderInterface> loader_app( new AppearanceComponentLoader() );
	EXPECT_TRUE(object_builder.RegisterLoader("TransformComponent", loader));
    EXPECT_TRUE(object_builder.RegisterLoader("Appearance", loader_app));
    EXPECT_EQ(object_builder.BuildObject(TEST_OBJECT_ID, "t21"), BUILD_INCOMPLETE);
    EXPECT_TRUE(object_builder.LoaderExists("TransformComponent"));
    EXPECT_TRUE(object_builder.LoaderExists("Appearance"));

	shared_ptr<TransformComponentInterface> transform_component = gObjectManager.QueryInterface<TransformComponentInterface>(TEST_OBJECT_ID, "TransformComponent");
	EXPECT_EQ(TEST_OBJECT_ID, transform_component->object_id());
	EXPECT_TRUE(transform_component->component_info().type == ComponentType("TransformComponent"));

    shared_ptr<AppearanceComponentInterface> appearance_component = gObjectManager.QueryInterface<AppearanceComponentInterface>(TEST_OBJECT_ID, "Appearance");
	EXPECT_EQ(TEST_OBJECT_ID, appearance_component->object_id());
	EXPECT_TRUE(appearance_component->component_info().type == ComponentType("Appearance"));
}

//
TEST_F(ObjectBuilderTest, BuildMultiComponentObjectWithMixedLoaders) {
}