/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "scripting_manager.h"

#include <iostream>
#include <fstream>
#include <boost/python.hpp>
#include <glog/logging.h>

using namespace std;
using namespace anh::scripting;
using namespace boost::python;

ScriptingManager::ScriptingManager(const string& base_path) 
{
    base_path_ = base_path;
    // initialize the python ScriptingManager
    Py_Initialize();
    // Retrieve the main module
    main_ = import("__main__");
    // Retrieve the main dictionary 'global' namespace if you will
    global_ = main_.attr("__dict__");
}
ScriptingManager::~ScriptingManager()
{
    loaded_files_.empty();
}
void ScriptingManager::load(const string& filename)
{
    try{              
        string input_str(&getFileInput_(filename)[0]);
        if (input_str.length() > 0)
        {
            auto file_str = make_shared<str>(input_str);
            loaded_files_.insert(make_pair(string(fullPath_(filename)), file_str));
        }
    }
    catch(...)
    {
        getExceptionFromPy();
    }
}
void ScriptingManager::run(const string& filename)
{
    // are you trying to run a file that's not loaded?
    // lets load the file and run it anyway
    if (!isFileLoaded(filename))
        load(filename);
    
    str loaded_file = getLoadedFile(filename);
    try
    {
        exec(loaded_file, global_, global_);
    }
    catch(...)
    {
        getExceptionFromPy();
    }
}
object ScriptingManager::embed(const string& filename, const string& return_name) {
    // are you trying to run a file that's not loaded?
    // lets load the file and run it anyway
    if (!isFileLoaded(filename))
        load(filename);
    
    str loaded_file = getLoadedFile(filename);
    object embed;
    try
    {
        exec(loaded_file, global_, global_);
        embed = main_.attr(return_name.c_str());
    }
    catch(...)
    {
        getExceptionFromPy();
    }
    return embed;
}
bool ScriptingManager::loadModules(std::vector<_inittab> modules)
{
    int failures = 0;
    for_each(modules.begin(), modules.end(), [modules, &failures](_inittab module) {
        if(PyImport_AppendInittab(module.name, module.initfunc) == -1) {
            LOG(WARNING) << "Module " << module.name << " could not be loaded";
            failures++;
            }
        });
    
    return failures == 0;
}
void ScriptingManager::reload(const string& filename)
{
    if (isFileLoaded(filename))
    {
        removeFile(filename);
    }
    load(filename);
}
void ScriptingManager::removeFile(const string& filename)
{
    auto it = loaded_files_.begin();
    for (; it != loaded_files_.end();)
    {
        if (it->first == fullPath_(filename))
        {
            loaded_files_.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}
bool ScriptingManager::isFileLoaded(const string& filename)
{
    auto it = find_if(loaded_files_.begin(), loaded_files_.end(), [this,&filename](bp_object_map::value_type& file){
        return file.first == fullPath_(filename);
    });
    return it != loaded_files_.end();
}
void ScriptingManager::setFullPath_(const string& filename)
{
    setFullPath_(filename, base_path_);
}
void ScriptingManager::setFullPath_(const string& filename, const string& root_path)
{
    full_path_.clear();
    full_path_.append(root_path);
    full_path_.append(filename);
}
str ScriptingManager::getLoadedFile(const string& filename)
{
    auto it = find_if(loaded_files_.begin(), loaded_files_.end(), [&](bp_object_map::value_type& file){
        return file.first == fullPath_(filename);
    });
    if (it != loaded_files_.end())
        return *it->second;
    else 
        return str();
}
char* ScriptingManager::fullPath_(const string& filename)
{
    setFullPath_(filename);
    return const_cast<char*>(full_path_.c_str());
}

vector<char> ScriptingManager::getFileInput_(const string& filename)
{
    vector<char> input;
    char* fullfile = fullPath_(filename);
    ifstream file(fullfile, ios::in);
    if (!file.is_open())
    {
        // set our error message here
        cerr << "Can't load file in path: " + string(fullfile) << endl;
        input.push_back('\0');
        return input;
    }
    file >> noskipws;
    copy(istream_iterator<char>(file), istream_iterator<char>(), back_inserter(input));
    input.push_back('\n');
    input.push_back('\0');
    
    return input;
}
void ScriptingManager::getExceptionFromPy()
{
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
    LOG(WARNING) << os.str();
}
