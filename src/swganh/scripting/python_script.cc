// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "python_script.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>

#include "swganh/logger.h"

#include <boost/python.hpp>
#include <Python.h>

#include "swganh/scripting/utilities.h"

using namespace swganh::scripting;
namespace bp = boost::python;

PythonScript::PythonScript(const std::string& filename)
        : filename_(filename)
{
    ReadFileContents_();    
    PreparePythonEnvironment_();
}

void PythonScript::Run()
{
	ScopedGilLock lock;

	try
    {
#ifdef _DEBUG
        ReadFileContents_();
#endif

        DLOG(info) << "Executing script: " << filename_;
        file_object_ = bp::exec(filecontents_.c_str(), globals_, globals_);
    }
    catch (bp::error_already_set&)
    {
		logPythonException();
    }
}

void PythonScript::PreparePythonEnvironment_()
{
    ScopedGilLock lock;

    try
    {
        bp::object main = bp::object(bp::handle<>(bp::borrowed(
            PyImport_AddModule("__main__")
        )));

        globals_ = main.attr("__dict__");
    }
    catch (bp::error_already_set&)
    {
		logPythonException();
    }
}

void PythonScript::ReadFileContents_()
{
    std::ifstream filestream(filename_);
    filestream >> std::noskipws;

    filecontents_ = std::string(
        (std::istreambuf_iterator<char>(filestream)),
        std::istreambuf_iterator<char>());
}
