// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "python_command_creator.h"

#include "swganh/logger.h"

using namespace swganh::command;


PythonCommandCreator::PythonCommandCreator(const std::string& script, const std::string& class_name)
    : script_(std::make_shared<swganh::scripting::PythonScript>(script))
    , class_name_(class_name)
{}

PythonCommandCreator::PythonCommandCreator(const PythonCommandCreator &)
{
    DLOG(info) << "WTF... Why You Copyin Willis?";
}

PythonCommandCreator::PythonCommandCreator(PythonCommandCreator&& other)
    : script_(nullptr)
    , class_name_("")
{
    script_ = other.script_;
    class_name_ = other.class_name_;

    other.script_ = nullptr;
    other.class_name_ = "";
}

std::shared_ptr<CommandInterface> PythonCommandCreator::operator() ()
{
    return script_->CreateInstance<CommandInterface>(class_name_);
}
