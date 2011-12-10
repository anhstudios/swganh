
#include "python_script.h"

#include <cstdio>
#include <fstream>
#include <iterator>

#include <glog/logging.h>

#include <boost/python.hpp>
#include <Python.h>

using namespace boost::python;
using namespace std;
using namespace swganh::scripting;

PythonScript::PythonScript(const string& filename)
        : filename_(filename)
{
    FILE* fp = fopen(filename.c_str(), "r");
    if (!fp)
    {
        throw runtime_error(filename + ": " + strerror(errno));
    }
    fclose(fp);

    Py_Initialize();
    
	try{
        object main = import("__main__");
        globals_ = main.attr("__dict__");

        globals_["context"] = dict();
    } 
    catch (error_already_set &) 
    {
        PyErr_Print();
    }   
}

void PythonScript::Run()
{
	try{
        ifstream file(filename_);
        vector<char> input;

        file >> std::noskipws;
        std::copy(istream_iterator<char>(file), istream_iterator<char>(), back_inserter(input));
        input.push_back(0);
        file_object_ = exec(&input[0], globals_, globals_);
    } 
    catch (error_already_set &) 
    {
        PyErr_Print();
    }   
}
