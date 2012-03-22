.. SWGANH Python Bindings documentation master file, created by
   sphinx-quickstart on Thu Mar 01 18:14:27 2012.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Introduction to Scripting
==================================================

General Overview:
The SWGANH project uses `Python 3.x <http://python.org/doc/>`_ for in game scripting. 
The most obvious area where scripting is used is within the in-game commands.
Almost everything in SWG is a command, and they can all be processed through scripting.
SWGANH has chosen Python as the preferred scripting language because of it's robustness, ease of use, and overall performance. 
Another reason is because the great C++ integration with the `Boost.Python <http://www.boost.org/doc/libs/1_49_0/libs/python/doc/index.html>`_ library.
This allows us to have very nice integration with Python from C++ and vice versa.

Why did SWGANH decide to use Scripting?
SWGANH uses scripting for ease of use and flexibility in game features.
What this means is that anyone who decides to use SWGANH can expect the ability to easily add, remove, and tweak gameplay with minimal changes to the core application of SWGANH.
This is something that was needed as SWGANH is first and foremost a platform for communities to gather around and build the Star Wars Galaxies game they have always wanted. 
While the base of the game is centered around Patch 14.1 (Pre-CU) we believe this allows for the highest amount of flexibility and overall generally enjoyment.
