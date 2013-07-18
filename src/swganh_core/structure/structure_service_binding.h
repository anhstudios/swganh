// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/logger.h"
#include "swganh/scripting/python_shared_ptr.h"

#include <memory>
#include <boost/python.hpp>
#include <boost/python/overloads.hpp>

#include "swganh_core/structure/structure_service_interface.h"

using namespace swganh::structure;
using namespace boost::python;

void exportStructureService()
{
	class_<StructureServiceInterface, std::shared_ptr<StructureServiceInterface>, boost::noncopyable>("StructureService", "The StructureService handles the process of placing/managing structures.", no_init)
		.def("enterStructurePlacementMode", &StructureServiceInterface::EnterStructurePlacementMode, "Puts the player into structure placement mode.")
		.def("placeStructureByDeed", &StructureServiceInterface::PlaceStructureByDeed, "Places a structure using the building from the given deed.");
}
