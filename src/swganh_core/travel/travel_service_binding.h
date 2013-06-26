// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include <vector>

#include "swganh/scripting/python_shared_ptr.h"
#include "travel_service_interface.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/call.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace swganh::travel;
using namespace swganh::object;
using namespace boost::python;
using namespace std;

void exportTravelService()
{
    class_<TravelServiceInterface, std::shared_ptr<TravelServiceInterface>, boost::noncopyable>("TravelService", "", no_init)
		.def("beginTicketTransaction", &TravelServiceInterface::BeginTicketTransaction, "")
		.def("useTicket", &TravelServiceInterface::UseTicket, "")
		.def("getAvailableTickets", &TravelServiceInterface::GetAvailableTickets, "")
		.def("getInventoryTicket", &TravelServiceInterface::GetInventoryTicket, "")
		.def("getAvailableInventoryTicketBySelection", &TravelServiceInterface::GetAvailableInventoryTicketBySelection, "")
		;
}
