// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <string>

#include <boost/python/object.hpp>

#include "swganh/scripting/utilities.h"

namespace swganh {
namespace scripting {
    class PythonScriptCreator
    {
    public:
        PythonScriptCreator(std::string module_name, std::string class_name);

        template<typename T>
        std::shared_ptr<T> operator() ()
        {
            ScopedGilLock lock;
            std::shared_ptr<T> interfaceObj = nullptr;
            try 
            {

        #ifdef _DEBUG
                module_ = boost::python::object(boost::python::handle<>(PyImport_ReloadModule(module_.ptr())));
        #endif
                
                auto new_instance = module_.attr(class_name_.c_str())();

                if (!new_instance.is_none())
                {
                    T* obj_pointer = boost::python::extract<T*>(new_instance);
                    interfaceObj.reset(obj_pointer, [new_instance] (T*) {});
                }
            }
			catch (boost::python::error_already_set&)
			{
				swganh::scripting::logPythonException();
			}

            return interfaceObj;
        }

    private:
        std::string module_name_;
        std::string class_name_;
        
        boost::python::object module_;
    };

}}
