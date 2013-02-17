// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/python_shared_ptr.h"
#include "badge_service_interface.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>

using namespace swganh::badge;
using namespace swganh::object;
using namespace boost::python;
using namespace std;

void exportBadgeService()
{
	typedef void (BadgeServiceInterface::*GiveBadgeByNameBinding)(std::shared_ptr<Object>, std::string);
	typedef void (BadgeServiceInterface::*GiveBadgeByIdBinding)(std::shared_ptr<Object>, uint32_t);
	typedef bool (BadgeServiceInterface::*HasBadgeByNameBinding)(std::shared_ptr<Object>, std::string);
	typedef bool (BadgeServiceInterface::*HasBadgeByIdBinding)(std::shared_ptr<Object>, uint32_t);
	typedef void (BadgeServiceInterface::*RemoveBadgeByNameBinding)(std::shared_ptr<Object>, std::string);
	typedef void (BadgeServiceInterface::*RemoveBadgeByIdBinding)(std::shared_ptr<Object>, uint32_t);

    class_<BadgeServiceInterface, std::shared_ptr<BadgeServiceInterface>, boost::noncopyable>("BadgeService", "The badge service gives and verifies player badges.", no_init)
		.def("giveBadge", GiveBadgeByNameBinding(&BadgeServiceInterface::GiveBadge), "Gives the specified player the specified badge if it exists.")
		.def("giveBadge", GiveBadgeByIdBinding(&BadgeServiceInterface::GiveBadge), "Gives the specified player the specified badge if it exists.")
		.def("hasBadge", HasBadgeByNameBinding(&BadgeServiceInterface::HasBadge), "Verifies if that player has the specified badge.")
        .def("hasBadge", HasBadgeByIdBinding(&BadgeServiceInterface::HasBadge), "Verifies if that player has the specified badge.")
		.def("removeBadge", RemoveBadgeByNameBinding(&BadgeServiceInterface::RemoveBadge), "Removes badge by name from target player.")
		.def("removeBadge", RemoveBadgeByIdBinding(&BadgeServiceInterface::RemoveBadge), "Removes badge by id from target player.")
		;
}
