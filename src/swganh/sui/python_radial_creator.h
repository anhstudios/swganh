// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SUI_PYTHON_RADIAL_CREATOR_H_
#define SWGANH_SUI_PYTHON_RADIAL_CREATOR_H_

#include <memory>
#include <string>

#include <boost/python/object.hpp>
namespace swganh {
namespace sui {

    class RadialInterface;
    class PythonRadialCreator
    {
    public:
        PythonRadialCreator(std::string module_name, std::string class_name);

        std::shared_ptr<RadialInterface> operator()();
		//template<typename T>
  //      std::shared_ptr<T> operator() ()
  //      {
  //          ScopedGilLock lock;
  //          std::shared_ptr<T> interfaceObj = nullptr;
  //          try 
  //          {

  //      #ifdef _DEBUG
  //              module_ = bp::object(bp::handle<>(PyImport_ReloadModule(module_.ptr())));
  //      #endif
  //              
  //              auto new_instance = module_.attr(class_name_.c_str())();

  //              if (!new_instance.is_none())
  //              {
  //                  T* obj_pointer = bp::extract<T*>(new_instance);
  //                  interfaceObj.reset(obj_pointer, [new_instance] (T*) {});
  //              }
  //          }
  //          catch(bp::error_already_set& /*e*/)
  //          {
  //              PyErr_Print();
  //          }

  //          return interfaceObj;
  //      }

    private:
        std::string module_name_;
        std::string class_name_;
        
        boost::python::object module_;
    };

}}

#endif  // SWGANH_SUI_PYTHON_RADIAL_CREATOR_H_
