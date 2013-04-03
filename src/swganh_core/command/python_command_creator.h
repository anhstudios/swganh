// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <string>

#include <boost/noncopyable.hpp>

#include "swganh/scripting/python_script.h"

#include "command_interface.h"

namespace swganh {
namespace command {

    class CommandInterface;

    class PythonCommandCreator : boost::noncopyable
    {
    public:
        PythonCommandCreator(const std::string& script, const std::string& class_name);
        PythonCommandCreator(PythonCommandCreator&& other);
        PythonCommandCreator(const swganh::command::PythonCommandCreator &);
        std::shared_ptr<CommandInterface> operator() ();

    private:
        PythonCommandCreator();

        std::string class_name_;        
        std::shared_ptr<swganh::scripting::PythonScript> script_;
    };

}}
