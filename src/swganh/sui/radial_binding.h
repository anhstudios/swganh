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
using namespace boost::python;
using namespace std;

namespace swganh {
namespace sui {

	struct RadialWrap : RadialInterface, wrapper<RadialInterface>
	{
		void BuildRadial(std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> target, std::vector<swganh::messages::controllers::RadialOptions> radials)
		{
			boost::python::list radials_python;
			for (auto& r : radials)
			{
				radials_python.append(r);
			}
			this->get_override("BuildRadial")(owner, target, radials_python);
		}
		void HandleRadial(std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> target, uint8_t action)
		{
			this->get_override("HandleRadial")(owner, target, action);
		}
	};

	void exportRadial()
	{
		class_<RadialWrap, boost::noncopyable>("RadialMenu", "A radial class purely used in python.", no_init)
			.def("BuildRadial", pure_virtual(&RadialWrap::BuildRadial), "Builds a radial for the target :class:`Object`")
			.def("HandleRadial", pure_virtual(&RadialWrap::HandleRadial), "Handles a specific radial action");		
	}
}}

#endif //SWGANH_SUI_RADIAL_BINDING_H_