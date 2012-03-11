
#ifndef SWGANH_SCRIPTING_PYTHON_SCRIPT_H_
#define SWGANH_SCRIPTING_PYTHON_SCRIPT_H_

#include <string>
#include <memory>

#include <boost/python.hpp>

namespace swganh {
namespace scripting {

    class PythonScript
    {
    public:
        PythonScript(const std::string& filename);

        void Run();

        template<typename T>
        void SetContext(const std::string& key, T value)
        {
	        try{
				globals_[key.c_str()] = value;
            } 
            catch (boost::python::error_already_set &) 
            {
                GetPythonException();
            }   
        }
		void SetImport(const std::string& import)
		{
			imports_ = import;
		}

        boost::python::object GetGlobals() { return globals_; }
    private:
        PythonScript();

        void GetPythonException();

        void ReadFileContents();

        std::string filename_;
        std::string filecontents_;
		std::string imports_;
        boost::python::object file_object_;
        boost::python::object globals_;
    };

}}  // namespace swganh::scripting

#endif  // SWGANH_SCRIPTING_PYTHON_SCRIPT_H_
