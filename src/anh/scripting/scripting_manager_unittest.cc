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


#include "scripting_manager.h"
#include <boost/python.hpp>
#include <anh/scripting/scripting_modules_unittest.h>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <cstdint>

using namespace anh::scripting;
using namespace anh::component;
using namespace boost::python;
using namespace std;

// this is used for embedding, so we can have our bindings in another file
void baseDerive();
void componentDerive();
void testPolicies();
BOOST_PYTHON_MODULE(embedded_hello)
{
    baseDerive();
}
BOOST_PYTHON_MODULE(embedded_component)
{
    anh::component::componentDerive();
}
BOOST_PYTHON_MODULE(test_policies)
{
    testPolicies();
}
namespace {
class ScriptingManagerTest : public ::testing::Test 
{
 protected:
     virtual void SetUp() 
     {
        e = std::make_shared<ScriptingManager>("scripts/unittests/");     
     }
     std::shared_ptr<ScriptingManager> e;
     std::vector<_inittab> modules;
    _inittab module;
};
TEST_F(ScriptingManagerTest, loadScript ) 
{
    e->load("test.py");
    EXPECT_TRUE(e->isFileLoaded("test.py"));
}

TEST_F(ScriptingManagerTest, runLoadedScript) 
{
    e->load("test.py");
    EXPECT_TRUE(e->isFileLoaded("test.py"));
    EXPECT_NO_THROW(e->run("test.py"));
}
TEST_F(ScriptingManagerTest, runNonLoadedScript) 
{
    EXPECT_NO_THROW(e->run("nonloadedscript.py"));
}
TEST_F(ScriptingManagerTest, runSecondLoadedScript)
{
    e->load("test.py");
    e->load("testRunSecond.py");
    EXPECT_TRUE(e->isFileLoaded("testRunSecond.py"));
    EXPECT_NO_THROW(e->run("testRunSecond.py"));
}
TEST_F(ScriptingManagerTest, cantLoadScript)
{
    EXPECT_NO_THROW(e->load("noscript.py"));
    EXPECT_FALSE(e->isFileLoaded("noscript.py"));
}
TEST_F(ScriptingManagerTest, loadSameFileTwice)
{
    e->load("test.py");
    e->load("test.py");
    e->run("test.py");
    EXPECT_TRUE(e->isFileLoaded("test.py"));
}
TEST_F(ScriptingManagerTest, loadFileOneDeep)
{
    e->load("test-deep/scripta.py");
    EXPECT_TRUE(e->isFileLoaded("test-deep/scripta.py"));
}
TEST_F(ScriptingManagerTest, getLoadedFiles)
{
    e->load("../scripta.py");
    e->load("../scriptb.py");
    e->load("test.py");
    e->load("testRunSecond.py");
    e->load("test-deep/scripta.py");
    EXPECT_EQ(uint32_t(5),e->getLoadedFiles().size());
}
TEST_F(ScriptingManagerTest, removeLoadedFile)
{
    e->load("test.py");
    EXPECT_TRUE(e->isFileLoaded("test.py"));
    e->removeFile("test.py");
    EXPECT_FALSE(e->isFileLoaded("test.py"));
}
TEST_F(ScriptingManagerTest, reloadFile)
{
    e->load("test.py");
    EXPECT_TRUE(e->isFileLoaded("test.py"));
    e->reload("test.py");
    EXPECT_TRUE(e->isFileLoaded("test.py"));
}
//TEST_F(ScriptingManagerTest, getValueFromPython)
//{
//    module.name = "embedded_hello";
//    module.initfunc = PyInit_embedded_hello;
//    modules.push_back(module);
//
//    // load modules
//    if (e->loadModules(modules))
//    {
//        object obj (e->embed("embedded_hello.py", "PythonDerived"));
//        object py_base = obj();
//        Base& py = extract<Base&>(py_base);
//        EXPECT_EQ("Hello from Python!", py.hello());
//    }
//}
/// CALL POLICIES TESTING

//Ties lifetime of Y.x to C++ argument to that of result
TEST_F(ScriptingManagerTest, getInternalReference)
{
    module.name = "test_policies";
    module.initfunc = PyInit_test_policies;
    modules.push_back(module);
    // load modules
    if (e->loadModules(modules))
    {
        Y testY(5);

        object obj (e->embed("internal_ref.py", "Y"));
        Y& py = extract<Y&>(obj);
        EXPECT_FALSE(5 == py.x);
    }
};

//makes a copy of the object to C++
TEST_F(ScriptingManagerTest, copyNonConstRef)
{
    module.name = "test_policies";
    module.initfunc = PyInit_test_policies;
    modules.push_back(module);
    // load modules
    if (e->loadModules(modules))
    {
        object obj (e->embed("nonConstRef.py", "Z"));
        nonConstRef& py = extract<nonConstRef&>(obj);
        // in python we've set the object to be 5 from the constructor
        EXPECT_TRUE(5 == py.x);
        // this is changing the python object itself by reference
        obj.attr("x") = 6;
        EXPECT_TRUE(6 == py.x);
    }
};
// pass an existing object to python, modify it and use it back in c++
TEST_F(ScriptingManagerTest, referenceExistingObject)
{
    module.name = "test_policies";
    module.initfunc = PyInit_test_policies;
    modules.push_back(module);
    // load modules
    if (e->loadModules(modules))
    {
        // create new game object
        GameObject* g = new GameObject(1,2,5.6f);
        GameObject g1(21,31,3.1415f);
        // pass existing object directly into the global dictionary
        e->global()["CGameObject"] = object(g);
        // pass by reference doesn't matter all values are 'copied' to python
        e->global()["CGameObject1"] = object(&g1);
        object obj (e->embed("GameObject.py", "GameObject"));
        int x = extract<int>(e->global()["CGameObject"].attr("x"));
        EXPECT_EQ(5 , x);
        // the c++ object stays the same
        EXPECT_EQ(1 , g->x);
        EXPECT_EQ(21, g1.x);
        // we can see here that g1.x has changed to 8, why is this?
        // g1 is initially created in C++ on the stack and it's x value is 21.
        // we run the script GameObject.py, which sets CGameObject1.x = 8
        // we then extract that object from the global dictionary and confirm it's value in C++ is 8.
        g1 = extract<GameObject>(e->global()["CGameObject1"]);
        EXPECT_EQ(8, g1.x);
        delete g;
    }
};

TEST_F(ScriptingManagerTest, getComponentFromPython)
{
    module.name = "embedded_component";
    module.initfunc = PyInit_embedded_component;
    modules.push_back(module);
    // load modules
    if (e->loadModules(modules))
    {
        object obj (e->embed("embedded_component.py", "Component"));
        object py_base = obj();
        BaseComponent& comp = extract<BaseComponent&>(py_base);
        EntityId id = comp.entity_id();
        EXPECT_EQ(0, id);
    }
}

TEST_F(ScriptingManagerTest, getMockComponentFromPython)
{
    module.name = "embedded_component";
    module.initfunc = PyInit_embedded_component;
    modules.push_back(module);
    // load modules
    if (e->loadModules(modules))
    {
        object obj (e->embed("radial_component.py", "MockComponent"));
        object py_base = obj();
        MockComponentInterface& comp = extract<MockComponentInterface&>(py_base);
        EntityId id = comp.entity_id();
        EXPECT_EQ(0, id);   
    }
}
} // anon namespace
