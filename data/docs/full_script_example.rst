
Full Start to Finish Scripting Example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will take you step by step on how to create a simple command script for SWGANH.
This assumes that you have followed the server setup instructions and it is working as expected.

For this example I will be doing a very simple command, but one that has interaction with another player.
`Add Friend <http://wiki.swganh.org/index.php/Addfriend_%282A2357ED%29>`_

Step 1 - Startup
================

Add the script reference to the command table in the galaxy database.
Open up your favorite mysql editor program, edit the 'command' table and look for the script_hook column.
Here we are going to be putting in the location of our python script that we have yet to create.
In this case we are going to put 'scripts/commands/add_friend.py' into the script hook for the addFriend row.

Step 2 - Basic Script
=====================

Now that this is done we can create our script. Open up your favorite python editor (I just use notepad++)
We know based on our documentation located `Here <http://swganh.anhstudios.com/docs/Player.html>` _
That we want to use the 'add_friend' function in the Player class.

.. NOTE::

	The way the SWG hierarchy works is that `Player <http://swganh.anhstudios.com/docs/Player.html>`_ is a seperate object than `Creature <http://swganh.anhstudios.com/docs/Creature.html>`_
	This means that the normal actor will not work to add a friend, we will need to get the `Player <http://swganh.anhstudios.com/docs/Player.html>`_ somehow.
	This is done through another function `get_player <http://swganh.anhstudios.com/docs/Creature.html#swgpy.object.Creature.get_player>`_, this will return the player object and then we can continue.

our script now looks like this...

.. code-block:: py3

    import swgpy.object

    # Get the player object
    player = actor.get_player()
    target_player = creature_target.get_player()
    # Make sure the player object and the target exists
    if player and target_player:
        # Check if the player target is already in our friends list
        if not player.is_friend(target_player.id):
            player.add_friend(target_player.id)
            actor.ObjectController().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_added', swgpy.ProseType.TT, target.id), False, False)
        elif:
            actor.ObjectController().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_duplicate', swgpy.ProseType.TT, target.id), False, False)
    elif:
        actor.ObjectController().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_not_found', swgpy.ProseType.TT, target.id), False, False)

A couple of things to note here

Number one is that we are getting both the player and the target player objects, and if these don't exist we display a message.
To determine which messages to send out, I used the `SWG String Search <http://strings.anhstudios.com/file/cmnty>`_ and searched for 'friend_added'
Secondly we are using the ObjectController to send the message to the actor to let them know the target was either added or not.
We used ProseType of TT, as that's what is shown in the `SWG String Search <http://strings.anhstudios.com/file/cmnty>`_ and passed it the 'target id of the creature'
The two False at the end just tell SendSystemMessage that we want to show it not just in the chatbox (also in the middle of the screen) and to only send it to the current actor.

Step 3 - First Example Finished?
================================

Lets save our script as add_friend.py in the data/scripts/commands folder
Build our server again (this will copy over the scripts from data/commands to bin/Debug/commands (if on Windows)
Then, we should be able to run our server and the command should execute as expected.

You are done with your first simple example!

Step 4 - Troubleshooting
========================

Or are you? The great thing about Python is you are able to tweak the script at runtime and see the results immediately during the next script run.
My first run through for example I got a somewhat cryptic message in the server console output
::

	Python error:
	SyntaxError: (`invalid syntax`, (`<string>`, 12, 6, `\telif:\n`))

Woah... what does this all mean? Python lets us know that it didn't like our script and something is definitely wrong here.
Lets break it up piece by piece.

*SyntaxError* This means python couldn't execute a particular line because it didn't know how to interpret it.
*12, 6* This is saying the error is on line 12 and in position 6
*`\telif:\n`*
Well that narrows it down pretty well for us, we know that line 12, we have a tab indentation and an elif: and a newline, which is what the \t and \n are respectively
This tells me that it is most likely a formatting error, or elif is not the correct syntax in this case.
In reality this is me just making a dumb mistake, I realize that I should be using else: instead of elif: as elif is used for
Once i replace the elif with else:
I get a different error, saying NoneType does not have method get_player
The problem here is that our creature_target doesn't exist. I need to re-look at how add_friend is supposed to work...
It looks like it just simply uses a name as a parameter.

Step 5 - Down the Rabbit Hole
=============================

Since this is the first social kind of command added to the game we are going to have to do more work than expected.
We will need to create a new service that is going to allow us to interact with the database, and interact with the player itself.

Lets start creating our new service. Go into swganh/src/ and I will create a new folder called 'social'
I need to create some files in here. I know I want to have a 'service' so I will create social_service.h and social_service.cc
I will also create a social_service_binding.h and social_service_binding.cc and a CMakeLists.txt
This should be all I need initially to get this service going.

For CMakeLists.txt I copied it from an existing service and changed the name to 'social' That's really all I need to do for that.
We also need to open up CMakeLists.txt in src/swganh and put in the following:
add_subdirectory(social)

This will allow our build system to recognize there is another CMakeLists.txt in the social folder and the parse it out accordingly.

Step 6 - Services In Depth
==========================

We need to take a step back here and really go over what a service is in the context of swganh and why we should be creating one.
Services are generally available the entire lifetime of the server. They can handle SWG Protocols and expose APIs for controlling/accessing data
that covers a feature set that is orthogonal to all other services.
This means that the service is available as long as the Server is operational, it exposes some functionality to other services and/or scripts.

Perect, this sounds like what we want. We want a service that is able to pull data from the database, have that data available the entire life of the server,
and we want to allow other services access to this data.

Step 7 - Service Integration and Playing with Data
==================================================

We have our existing social_service files and social_service_binding files, these are probably empty, so let's get these filled in.
Lets start with social_service.h

As a header file, this describes what we are going to be doing with our service. Based on our services we can see that this service
is going to inherit from swganh::base::BaseService. Our code looks little like this *snippet*
::

	class SocialService : public swganh::base::BaseService
    {
    public:
        explicit SocialService(anh::app::KernelInterface* kernel);

        ~SocialService();

        anh::service::ServiceDescription GetServiceDescription();


    private:

        void onStart();

    };

This sets up a very very basic structure, all we are doing here is just getting the service created, we will flesh it out later.
Lets just do the same for the .cc file
::

	SocialService::SocialService(KernelInterface* kernel)
    : BaseService(kernel)
	{}

	SocialService::~SocialService()
	{}

	ServiceDescription SocialService::GetServiceDescription()
	{
		ServiceDescription service_description(
			"SocialService",
			"social",
			"0.1",
			"127.0.0.1",
			0,
			0,
			0);

		return service_description;
	}

	void SocialService::onStart()
	{

	}

Ok, basic structure is in place.
Let's get enough just to set up our friends list.
We know we will need to have a function that is called AddFriend takes in a Player object and a string as parameters and returns a true or false, if the player was found or not.
so lets do that...
::

	bool SocialService::AddFriend(const shared_ptr<Player>& player, const string& friend_name)
	{
		return true;
	}


Right now our function does nothing and just returns true.
What we want to do is build out a simple interface as we know that this social service will end up doing a lot more than just adding a friend.
We know that we need to interact with the database and get some data, we can and should do this using a plugin. More about plugins and how they interact `HERE <>_`

Step 8 - Setting up the Database Provider and Interface
=======================================================
The provider interface is a pretty common pattern in our codebase, so lets do what we usually do and take a look at an example. in swganh/character/character_provider_interface.h
we can see a very similar thing to what we will need to do. In fact actually it looks like what we really want to do is just add to the character_provider_interface a function
that will perform the required action. What we really want is a way to look up a character name to see if they exist, this sounds just like something the character provider should do for us.

Lets open the character_provider_interface.h and add a few lines.
::

	virtual uint64_t GetCharacterIdByName(const std::string& name) = 0;

Ok, now we need to update the existing mysql_character plugin to take into consideration this change.

our mysql_character_provider.h has added this.
::

	virtual uint64_t GetCharacterIdByName(const std::string& name);

and our mysql_character_provider.cc has filled in the details for this function.
::

	uint64_t MysqlCharacterProvider::GetCharacterIdByName(const string& name)
	{
		uint64_t character_id = 0;
		try {
			auto conn = kernel_->GetDatabaseManager()->getConnection("galaxy");
			auto statement = std::unique_ptr<sql::PreparedStatement>(
				conn->prepareStatement("SELECT id FROM object where custom_name like ? and type_id = ?;")
				);
			statement->setString(1, name + '%');
			statement->setUInt(2, swganh::object::player::Player::type);
			auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
			if (result_set->next())
			{
			   character_id = result_set->getUInt64(1);
			}

		} catch(sql::SQLException &e) {
			LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
			LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
		}
		return character_id;
	}

Now lets set up the interaction with the character provider plugin and see what this looks like.
::

	bool SocialService::AddFriend(const shared_ptr<Player>& player, const string& friend_name)
	{
		uint64_t friend_id = character_provider_->GetCharacterIdByName(friend_name);
		/// If we found our friend, lets add them to our friends list (which will get updated by the player)
		if (friend_id > 0)
		{
			player->AddFriend(friend_name);
			// This persists the player object immediately.
			kernel()->GetServiceManager()->GetService<swganh::simulation::SimulationService>
				("SimulationService")->PersistObject(player->GetObjectId());

			return true;
		}

		return false;
	}

That is quite easy to follow and very modular, we can change what we are using to actually get the Character Id without having to rework this code.
This is a huge advantage of interfaces and why you see them in our codebase so frequently. Please check out the full social_service.h and .cc for more details.

Step 9 - Registering the Service
================================

Now that we have a service created and compiling, we can add it to our server startup process.
Lets open up swganh_app.cc this is quite a large file and really does a lot of the work of starting up the game server.
first we need to 'include' our file that we created, so the swganh app knows about it.
#include "swganh/social/social_service.h"

Next lets go down to where all the other services are loaded: LoadCoreServices.

Lets add this in under the last service there:
::

	kernel_->GetServiceManager()->AddService(
            "SocialService",
            make_shared<social::SocialService>(kernel_.get()));

Now we'll build the server, all should be good.

Step 10 - Setting up bindings
=============================

Now that we've created a very simple service with a very simple API, we want to expose this to Python to use in our script.
We do this through a process called binding. Fortunately most of the hard work is done for us with Boost.Python
All we need to do is tell Boost.Python about our service and it will automatically create a module for us.
Lets see what that looks like now. This is social_service_binding.h
::

	#include "anh/python_shared_ptr.h"
	#include "social_service.h"

	#include <boost/python.hpp>

	using namespace swganh::social;
	using namespace boost::python;
	using namespace std;

	void exportSocialService()
	{
		class_<SocialService, shared_ptr<SocialService>, boost::noncopyable>("SocialService", "The social service handles services that involve social actions", no_init)
			.def("add_friend", &SocialService::AddFriend, "Checks the database to see if the character name exists and then adds the friend to the player")
			;
	}

As you can see this is a very simple example, we are using Boost.Python to basically create a python module which describes this C++ class and methods.
Most services are going to be very similar to this, so this is a good template to go off.
As you can see we have added in our 'AddFriend' method, this is to be expose to python as "add_friend"

There is just one more step in order for this binding to work properly...
We need to now add a way to get to this service. This is done in the ANH Core through a system called the Service Manager.
Luckily we have an example to pull from on how to expose a service through this.
We will be opening up swganh_kernel_binding.h in app_binding.

This will be used to expose all services out to Python. We will be using the SimulationService as an example to copy from.
::

	    class_<anh::service::ServiceManager, boost::noncopyable>("ServiceManager", "provides an interface to common services", no_init)
        .def("simulation_service", make_function(
               bind(&anh::service::ServiceManager::GetService<swganh::simulation::SimulationService>, std::placeholders::_1, "SimulationService"),
                default_call_policies(),
                boost::mpl::vector<std::shared_ptr<swganh::simulation::SimulationService>, anh::service::ServiceManager*>()),
                "returns an internal refrence of the :class:`.SimulationService`")

This is actually pretty complicated code and there is a lot of magic going on behind the scenes, but all we need to know is that we are exposing the
service to python as a shared_ptr. We can literally replace simulation with social and this will work as expected.

Step 11 - Back to the script!
=============================

Ok, so now we know that we need to use the social service that we set up to add a friend. We also know through our documentation that we can
get to services through the :class:`.SWGKernel` service_manager
So lets add that call in the script after we check to see if the name is already in our friends list.
Next we need to send the player a message if the command succeeded or not. From the documentation again, we see that in order to send messages to the client
we need to get the Controller of the object and send a message.
The syntax is this
::

	actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_added', swgpy.ProseType.TT, friend_request_name), False, False)

This is telling the code to get the controller object, invoke the SendSystemMessage using it's OutOfBand constructor that we are building right into the command.
We are telling it that it's a TT ProseType type, which we can see by looking `SWG Strings cmnty <http://strings.anhstudios.com/file/cmnty>`_
This fits right in with what the Strings message says we want to do.
Building out our script a little more it now looks like this
::

	import re, swgpy.object

	split = re.split('\W+', command_string)
	friend_request_name = split[0]
	print(friend_request_name)
	# Get the player object
	player = actor.get_player()
	if player:
		# Check if the name is already in our friends list
		if not player.is_friend(friend_request_name):
			added = kernel.service_manager().social_service().add_friend(player, friend_request_name)
			if added:
				print(added)
				actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_added', swgpy.ProseType.TT, friend_request_name), False, False)
			else:
				print(added)
				actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_not_found', swgpy.ProseType.TT, friend_request_name), False, False)
		else:
			print(added)
			actor.Controller().SendSystemMessage(swgpy.OutOfBand('cmnty', 'friend_duplicate', swgpy.ProseType.TT, friend_request_name), False, False)
	else:
		print('Player object not found for object id' + actor.id)

And we run it in our server and now we get a response, saying that our friend was added (if he exists on the server)!

There are more things to keep in mind such as checking the ignore list, checking to see if the added friend is online and sending a message update.
This example will not cover those scenarios but you can check them out in the existing add_friend.py script.

This example was more of a step by step process that followed MY particular though pattern. There can be other ways of doing things, I just
hope this was helpful. For any particular or in-depth questsions, please hit us up on IRC@ irc.swganh.com #swganh

~Kyle Craviotto