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

PythonScript::PythonScript(const std::string& filename, bool delay_execution)
        : filename_(filename)
        , globals_(bp::dict())
{
    ReadFileContents_();    
    PreparePythonEnvironment_();

    if (!delay_execution)
    {
        Run();
    }
}

void PythonScript::Run()
{
	ScopedGilLock lock;

	try
    {
#ifdef _DEBUG
        ReadFileContents_();
#endif

        file_object_ = bp::exec(filecontents_.c_str(), globals_, globals_);
    }
    catch (bp::error_already_set&)
    {
		logPythonException();
    }
}


std::shared_ptr<boost::python::object> PythonScript::GetGlobal(const std::string& name)
{
    std::shared_ptr<boost::python::object> instance = nullptr;

    ScopedGilLock lock;

    try
    {
        instance = std::shared_ptr<boost::python::object>(
            new boost::python::object(globals_[name.c_str()]),
            [] (boost::python::object* obj) { ScopedGilLock lock; delete obj; });            
    } 
    catch(boost::python::error_already_set&) 
    {
        swganh::scripting::logPythonException();
    }   

    return instance;
}

boost::python::dict PythonScript::GetGlobals()
{
    return bp::extract<bp::dict>(globals_);
}

void PythonScript::PreparePythonEnvironment_()
{
    ScopedGilLock lock;

    try
    {
        main_ = boost::python::object (boost::python::handle<>(boost::python::borrowed(
            PyImport_AddModule("__main__")
        )));

        globals_ = main_.attr("__dict__");
        //globals_["__builtins__"] = tmp["__builtins__"];
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
