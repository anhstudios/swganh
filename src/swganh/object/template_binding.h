#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/logger.h"
#include "swganh/python_shared_ptr.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/call.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

#include "swganh/object/object_binding.h"

#include "template_interface.h"

#include "swganh/scripting/utilities.h"

namespace bp = boost::python;
using namespace std;	
using swganh::scripting::ScopedGilLock;

namespace swganh {
namespace object {

	void InsertMapTemplate(
		std::map<std::string, std::shared_ptr<TemplateInterface>> map_,
		std::string str, std::shared_ptr<TemplateInterface> template_)
	{
		map_.insert(make_pair(str, template_));
	}

	struct TemplateWrap : TemplateInterface, bp::wrapper<TemplateInterface>
	{
		TemplateWrap(PyObject* obj)
			: self_(bp::handle<>(bp::borrowed(obj)))
		{
			ScopedGilLock lock;
			bp::detail::initialize_wrapper(obj, this);
		}

		std::shared_ptr<Object> CreateTemplate(std::map<std::string, std::string> parameters)
		{
			ScopedGilLock lock;
			try 
			{
				return this->get_override("create")(parameters).as<std::shared_ptr<Object>>();
			}
			catch (bp::error_already_set&)
			{
				PyErr_Print();
			}
			return make_shared<Object>();
		}
	private:
		bp::object self_;
	};

	void exportObjectTemplate()
	{
		bp::class_<TemplateInterface, TemplateWrap, std::shared_ptr<TemplateInterface>, boost::noncopyable>("BaseTemplate")
			.def("create", &TemplateWrap::CreateTemplate)			
		;		
		bp::class_<std::map<std::string, std::string>>("StringMap", "Map of Strings for Template Params")
			.def(bp::map_indexing_suite<std::map<std::string, std::string>>())
		;	
		bp::class_<std::map<std::string, std::shared_ptr<Object>>>("PrototypeMap", "Map of objects")
			.def(bp::map_indexing_suite<std::map<std::string, std::shared_ptr<Object>>, true>())
		;
		bp::class_<std::map<std::string, std::shared_ptr<TemplateInterface>>>("TemplateMap", "Map of Templates")
			.def(bp::map_indexing_suite<map<std::string, std::shared_ptr<TemplateInterface>>, true>())
			.def("Insert", InsertMapTemplate)
		;
	}
}}
