// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/scripting/python_shared_ptr.h"
#include "map_service_interface.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>

using namespace swganh::map;
using namespace boost::python;
using namespace std;

void exportMapService()
{
	// AddLocation
	typedef void (MapServiceInterface::*AddLocationBySceneName)(std::string, std::wstring, float, float, uint32_t, uint32_t);
	typedef void (MapServiceInterface::*AddLocationBySceneId)(uint32_t, std::wstring, float, float, uint32_t, uint32_t);
	
	// RenoveLocation
	typedef void (MapServiceInterface::*RemoveLocationBySceneName)(std::string, std::wstring);
	typedef void (MapServiceInterface::*RemoveLocationBySceneId)(uint32_t, std::wstring);
	
	// LocationExists
	typedef bool (MapServiceInterface::*LocationExistsBySceneName)(std::string, std::wstring);
	typedef bool (MapServiceInterface::*LocationExistsBySceneId)(uint32_t, std::wstring);

    class_<MapServiceInterface, std::shared_ptr<MapServiceInterface>, boost::noncopyable>("MapService", "The map service manages map locations for each scene.", no_init)
		.def("addLocation", AddLocationBySceneName(&MapServiceInterface::AddLocation), "")
		.def("addLocation", AddLocationBySceneId(&MapServiceInterface::AddLocation), "")
		.def("removeLocation", RemoveLocationBySceneName(&MapServiceInterface::RemoveLocation), "")
		.def("removeLocation", RemoveLocationBySceneId(&MapServiceInterface::RemoveLocation), "")
		.def("locationExists", LocationExistsBySceneId(&MapServiceInterface::LocationExists), "")
		.def("locationExists", LocationExistsBySceneName(&MapServiceInterface::LocationExists), "")
		;
}
