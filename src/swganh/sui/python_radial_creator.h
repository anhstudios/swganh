// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

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
    private:
        std::string module_name_;
        std::string class_name_;
        
        boost::python::object module_;
    };

}}
