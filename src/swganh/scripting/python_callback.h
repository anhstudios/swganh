// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SCRIPTING_PYTHON_CALLBACK_H_
#define SWGANH_SCRIPTING_PYTHON_CALLBACK_H_

#include <boost/python/object.hpp>

#include "swganh/command/command_interface.h"

namespace swganh {
namespace scripting {

    class PythonCallback : public swganh::command::CommandCallbackInterface
    {
    public:
        PythonCallback(
            boost::python::object function,
            float delay_timer);

        PythonCallback(
            boost::python::object function,
            boost::python::object instance,
            float delay_timer);

        float GetDelayTimeInMs() const;

        void operator() ();

    private:
        boost::python::object function_;
        boost::python::object instance_;
        float delay_timer_;
    };

}}

#endif  // SWGANH_SCRIPTING_PYTHON_CALLBACK_H_
