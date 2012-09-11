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

using namespace boost::python;
using namespace std;
using namespace swganh::scripting;

PythonScript::PythonScript(const string& filename)
        : filename_(filename)
{
    ReadFileContents();
    swganh::scripting::ScopedGilLock lock;

	try
    {
        PyRun_SimpleString("import sys; sys.path.append('.');");

		boost::python::object main = boost::python::object (boost::python::handle<>(boost::python::borrowed(
			PyImport_AddModule("__main__")
		)));

        globals_ = main.attr("__dict__");
        globals_["swgpy"] = boost::python::import("swgpy");
        globals_["context"] = dict();
    }
    catch (error_already_set &)
    {
		ScopedGilLock lock;
        PyErr_Print();
    }
}

void PythonScript::Run()
{
	swganh::scripting::ScopedGilLock lock;
	try
    {
#ifdef _DEBUG
        ReadFileContents();
#endif
        LOG(info) << "Executing script: " << filename_;
        file_object_ = exec(filecontents_.c_str(), globals_, globals_);
    }
    catch (error_already_set &)
    {
        GetPythonException();
    }
}

void PythonScript::ReadFileContents()
{
    ifstream filestream(filename_);
    filestream >> noskipws;

    filecontents_ = string(
        (istreambuf_iterator<char>(filestream)),
        istreambuf_iterator<char>());
}

void PythonScript::GetPythonException()
{
    swganh::scripting::ScopedGilLock lock;

    std::ostringstream os;
    os << "Python error:\n  " << std::flush;

    PyObject *type = 0, *val = 0, *tb = 0;
    PyErr_Fetch(&type, &val, &tb);
    handle<> e_val(val), e_type(type), e_tb(allow_null(tb));

    try {
        object t = extract<object>(e_type.get());
        object t_name = t.attr("__name__");
        std::string typestr = extract<std::string>(t_name);

        os << typestr << std::flush;
    } catch (error_already_set const &) {
        os << "Internal error getting error type:\n";
        PyErr_Print();
    }

    os << ": ";

    try {
        object v = extract<object>(e_val.get());
        std::string valuestr = extract<std::string>(v.attr("__str__")());
        os  << valuestr << std::flush;
    } catch (error_already_set const &) {
        os << "Internal error getting value type:\n";
        PyErr_Print();
    }

    if (tb) {
        try {
            object tb_list = import("traceback").attr("format_tb")(e_tb);
            object tb_str = str("").attr("join")(tb_list);
            std::string str = extract<std::string>(tb_str);

            os << "\nTraceback (recent call last):\n" << str;
        } catch (error_already_set const &) {
            os << "Internal error getting traceback:\n";
            PyErr_Print();
        }
    } else {
        os << std::endl;
    }
    PyErr_Clear();
    std::cerr << os.str() << endl;
    LOG(warning) << os.str();
}