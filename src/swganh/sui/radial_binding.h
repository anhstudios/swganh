#ifndef SWGANH_SUI_RADIAL_BINDING_H_
#define SWGANH_SUI_RADIAL_BINDING_H_


#ifndef WIN32
#include <Python.h>
#endif

#include "anh/logger.h"
#include "anh/python_shared_ptr.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/call.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "radial_interface.h"

#include "swganh/scripting/utilities.h"

#include "pub14_core/messages/controllers/object_menu_request.h"
#include "pub14_core/messages/controllers/object_menu_response.h"



using namespace swganh::sui;
using namespace swganh::messages::controllers;
namespace bp = boost::python;
using namespace std;
using swganh::scripting::ScopedGilLock;

namespace swganh {
namespace sui {

	struct RadialWrap : RadialInterface, bp::wrapper<RadialInterface>
	{
		void BuildRadial(std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> target, std::vector<RadialOptions> radials)
		{
			try 
			{
				bp::list radials_python;
				for (auto& r : radials)
				{
					radials_python.append(r);
				}
				ScopedGilLock lock;

				this->get_override("BuildRadial")(owner, target, radials);
			}
			catch (bp::error_already_set& )
			{
				ScopedGilLock lock;
				PyErr_Print();
			}
		}
		void HandleRadial(std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> target, uint8_t action)
		{
			try 
			{
				ScopedGilLock lock;
				this->get_override("HandleRadial")(owner, target, action);
			}
			catch (bp::error_already_set& )
			{
				ScopedGilLock lock;
				PyErr_Print();				
			}
		}
	};

	void exportRadial()
	{
		bp::class_<RadialWrap, boost::noncopyable>("RadialMenu", "A radial class purely used in python.")
			.def("BuildRadial", bp::pure_virtual(&RadialWrap::BuildRadial), "Builds a radial for the target :class:`Object`")
			.def("HandleRadial", bp::pure_virtual(&RadialWrap::HandleRadial), "Handles a specific radial action");

		bp::class_<RadialOptions>("RadialOptions", "class defining the options needed for radials", 
			bp::init<uint8_t, uint8_t, uint8_t, std::wstring>())
			.def_readwrite("parent_item", &RadialOptions::parent_item, "the parent item of this radial")
			.def_readwrite("identifier", &RadialOptions::identifier, "the :class:`RadialIndentifier` this radial is")
			.def_readwrite("action", &RadialOptions::action, "the action id this radial should apply to")
			.def_readwrite("description", &RadialOptions::custom_description, "only needed if action is 3");			
					
		bp::class_<std::vector<RadialOptions>>("RadialOptionsList", "vector for the radials options")
			.def(bp::vector_indexing_suite<std::vector<RadialOptions>>());
	}
}}

#endif //SWGANH_SUI_RADIAL_BINDING_H_