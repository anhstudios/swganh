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

#include "radial_interface.h"

#include "swganh/scripting/utilities.h"

#include "pub14_core/messages/controllers/radial_options.h"


using namespace swganh::sui;
namespace bp = boost::python;
using namespace std;
using swganh::scripting::ScopedGilLock;

namespace swganh {
namespace sui {

	struct RadialWrap : RadialInterface, wrapper<RadialInterface>
	{
		void BuildRadial(std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> target, std::vector<swganh::messages::controllers::RadialOptions> radials)
		{
			try 
			{
				bp::list radials_python;
				for (auto& r : radials)
				{
					radials_python.append(r);
				}
				ScopedGilLock lock;
				this->get_override("BuildRadial")(owner, target, radials_python);
			}
			catch (bp::error_already_set& )
			{
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
				PyErr_Print();
			}
		}
	};

	void exportRadial()
	{
		bp::class_<RadialWrap, boost::noncopyable>("RadialMenu", "A radial class purely used in python.")
			.def("BuildRadial", pure_virtual(&RadialWrap::BuildRadial), "Builds a radial for the target :class:`Object`")
			.def("HandleRadial", pure_virtual(&RadialWrap::HandleRadial), "Handles a specific radial action");		
	}
}}

#endif //SWGANH_SUI_RADIAL_BINDING_H_