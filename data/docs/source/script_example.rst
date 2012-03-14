.. SWGANH Python Bindings documentation master file, created by
   sphinx-quickstart on Thu Mar 01 18:14:27 2012.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Scripting Example
==================================================

::

	if not actor.has_state(swganh.ACTION.COMBAT):
		actor.toggle_state_off(swganh.ACTION.PEACE);
		actor.toggle_state_on(swganh.ACTION.COMBAT)
		actor.activate_auto_attack()

Explanation:
This is a simple command that gets executed from a message that is sent to the server from the client. In this proceses the SWGANH server, adds game objects to the currently executing script. The objects are explained below in quotes.

*actor* The current Creature Object that is performing this command. See :class:`.Creature` for more info

*target* The target Tangible Object of the command that is being performed against. See :class:`.Tangible` for more info

*creature_target* The target Creature Object the command is being performed against. Only valid if the target is a :class:`.Creature`

*kernel* The Kernel is the main object of the SWGANH server, From here you can access game services, retrieve information about the server and more. See :class:`.SWGKernel`

