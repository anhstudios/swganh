// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "python_callback.h"

using swganh::scripting::PythonCallback;

PythonCallback::PythonCallback(
    boost::python::object function,
    float delay_timer)
{}

PythonCallback::PythonCallback(
    boost::python::object function,
    boost::python::object instance,
    float delay_timer)
{}

float PythonCallback::GetDelayTimeInMs() const
{
    return delay_timer_;
}

void PythonCallback::operator() ()
{
    if (!instance_.is_none())
    {
        function_();
    }
    else
    {
        function_(instance_);
    }
}
