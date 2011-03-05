## scripting - An open source python 3 scripting engine ##
====================================================================

This project uses the boost python library to help with a lot of the messiness dealing with python behind the scenes, references etc.
The goal of this scripting engine is to be able to use python within C++. Boost Python takes care of extending very well, but it doesn't
do much when you want to run python scripts within your c++ engine and then use the values from python.

*Getting Started:*
The scripting engine within libanh is really meant to be used within a server environment, but it is generic enough that use for other purposes should work.

The ScriptingManager has been set up in a way so you can easily load/run/unload/reload python scripts from a given directory. That directory
is set when you initialize the ScriptingManager object. From there you can individually load each script, or you can load an entire directory of scripts.
What 'loading' does in the context of the ScriptingManager is take the Python scripts read them into a Python string and save them in memory so they
can be run at a later point without needing to load the script again and then run the scripts which takes more processing time. 

*Running the python script will first check to see if the script is loaded, if it is not, it will load the script and then run it.*

Lets take a look at a simple example of loading a python script and then running it.
We heavily use the C++0x working standard in this project so I would recommend using smart pointers whenever able.

    // lets create our ScriptingManager and pass it a directory that we want to use, 
    //this can be a full directory or just within the library you are using.

    auto script_engine = std::make_shared<ScriptingManager>("scripts");
    // load up one of our scripts
    script_engine->load("script.py");
    //run the script
    script_engine->run("script.py");


This runs script.py. Pretty simple right? Sure, but how can we actually get integration with C++ and pass values back and forth?

Each class or function that we want to use in Python needs to be *wrapped* there are examples of this in the unit test and there are
lots of intricaces that go along with this, but they can be found in the [boost python doc][1].
After they've been wrapped and formed into modules, we can load them directly into python. The other way to do it is to load them into compiled
files (dlls) named .pyd, then these can be loaded into the python engine for use in scripts.

Load modules is a way that we can take a c++ converted module load it into the python dictionary and then utilize it within C++.

The other option from just running a python script directly is to *embed* it. What this does is executes the given script and then returns the object
that you are requesting. Let me give a quick example of how this works and it will make sense to why it makes sense.

Lets take a look at our unit tests in 'scripting_manager_unittest.cc' under "referenceExistingObject".

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
    }
and our python script
	from test_policies import *
	print(GameObject.z)
	print (CGameObject.x, CGameObject.y, CGameObject.z)
	CGameObject.x = 5
	print(CGameObject.x)
	print(CGameObject1.x,CGameObject1.y,CGameObject1.z)
	CGameObject1.x = 8
	print(CGameObject1.x)

	GameObject = CGameObject
	print(GameObject.z)
	
Let me explain a bit more than what the comments are telling us here. First we are setting the 'module' which is a struct that tells our loadModules
where it can find the needed information to do it's job. Then we are creating two GameObjects one is new'd up on the heap and the other on the stack.
We pass the objects to Python's global dictonary for use in the script. We then print out some of the values of CGameObject and CGameObject1 to make
sure they were passed to python correctly, when running the script you will se they do correctly. We then show that you can modify the objects values
inside python and when extracting the object back out in C++ we will see the updated values. This is incredibly helpful as we can pass off objects
to python, execute the script and then retrieve the values later for use in C++.

This becomes even more powerful in the next example when we can define an entire class in python and extract that object for use in C++. This allows
for rapid prototyping of classes in python. The only caveat is there must be an interface to extract the object to within C++.

As this project progresses we will have more concrete examples of how this engine is being used and it might even be tweaked a little bit to meet
our needs. If you have any questions please let me know at kyle@anhstudios.com

[1]: http://www.boost.org/doc/libs/1_46_0/libs/python/doc/ "boost python documentation"
[Galaxy]: http://anhstudios.com/galaxy/
[project website]: http://swganh.com/