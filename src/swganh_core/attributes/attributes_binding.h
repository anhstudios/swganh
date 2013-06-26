#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/logger.h"
#include "swganh/scripting/python_shared_ptr.h"
#include "swganh/app/swganh_kernel.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/call.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "attribute_template_interface.h"

#include "swganh/scripting/utilities.h"

#include "swganh_core/messages/attribute_list_message.h"

using namespace swganh::attributes;
using namespace swganh::messages;
namespace bp = boost::python;
using namespace std;	
using swganh::scripting::ScopedGilLock;

namespace swganh {
namespace attributes {

	struct AttributeTemplateWrap : BaseAttributeTemplate, bp::wrapper<BaseAttributeTemplate>
	{
		AttributeTemplateWrap(PyObject* obj)
			: self_(bp::handle<>(bp::borrowed(obj)))
		{
			ScopedGilLock lock;
			bp::detail::initialize_wrapper(obj, this);
		}

		AttributeListMessage BuildAttributeTemplate(std::shared_ptr<swganh::object::Object> object)
		{
			ScopedGilLock lock;

			try 
			{
				return this->get_override("buildAttributeTemplate")(object);
			}
			catch (bp::error_already_set&)
			{
				swganh::scripting::logPythonException();
			}

			return AttributeListMessage();
		}
	private:
		bp::object self_;
	};

	void exportAttributes()
	{
        bp::class_<AttributeTemplateInterface, boost::noncopyable>("AttributeTemplateInterface", bp::no_init)
			.def("buildAttributeTemplate", bp::pure_virtual(&AttributeTemplateInterface::BuildAttributeTemplate))
			.def("getKernel", bp::pure_virtual(&AttributeTemplateInterface::GetKernel), bp::return_internal_reference<>())
        ;

		bp::class_<BaseAttributeTemplate, AttributeTemplateWrap, bp::bases<AttributeTemplateInterface>, boost::noncopyable>("BaseAttributeTemplate")
			.def("buildAttributeTemplate", bp::pure_virtual(&BaseAttributeTemplate::BuildAttributeTemplate))
			.def("getKernel", &AttributeTemplateWrap::GetKernel, bp::return_internal_reference<>())
		;
		bp::class_<AttributeListMessage, boost::noncopyable>("AttributeListMessage", "Message that describes attributes for a given class::`Object`")
			.def_readwrite("object_id", &AttributeListMessage::object_id, "object_id that the attributes are describing")
			.def_readwrite("attributes", &AttributeListMessage::attributes, "attributes that describe the object")
		;
		bp::class_<Attribute, boost::noncopyable>("Attribute", "Object that describes an attribute", bp::init<std::string, std::wstring>())
			.def_readwrite("name", &Attribute::name, "name of the attribute ex: original_name")
			.def_readwrite("value", &Attribute::value, "value of the attribute")
		;
		bp::class_<vector<Attribute>>("AttributeList", "List for attributes data")
			.def(bp::vector_indexing_suite<vector<Attribute>, true>());
		;
	}
}}
