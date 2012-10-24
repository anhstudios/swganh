// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#include "fsm_manager.h"
#include "finite_state_machine_interface.h"
#include "fsm_state_interface.h"

#include "swganh_core/object/object.h"


using namespace swganh;
using namespace swganh::spawn;

FsmManager::FsmManager(EventDispatcher* event_dispatch)
	: dispatch_(event_dispatch)
{
}
	
void FsmManager::RegisterAutomaton(const std::wstring& name, std::shared_ptr<FiniteStateMachineInterface> automaton)
{
	boost::lock_guard<boost::shared_mutex> lock(mutex_);
	machines_.insert(std::make_pair(name, automaton));
}

std::shared_ptr<FiniteStateMachineInterface> FsmManager::GetAutomatonByName(const std::wstring& name)
{
	boost::shared_lock<boost::shared_mutex> lock(mutex_);
	auto find_itr = machines_.find(name);
	if(find_itr != machines_.end())
	{
		return find_itr->second;
	}
	return nullptr;
}

void FsmManager::DeregisterAutomaton(const std::wstring& name)
{
	boost::lock_guard<boost::shared_mutex> lock(mutex_);
	auto find_itr = machines_.find(name);
	if(find_itr != machines_.end())
	{
		machines_.erase(find_itr);
	}
}
		
void FsmManager::StartManagingObject(std::shared_ptr<swganh::object::Object> object, std::wstring machine)
{
	auto new_machine = GetAutomatonByName(machine);
	if(new_machine)
	{
		new_machine->StartManagingObject(object);
		object->SetAttribute("fsm_name", machine);
	}
}

void FsmManager::StopManagingObject(std::shared_ptr<swganh::object::Object> object)
{
	std::wstring machine_name = object->GetAttribute<std::wstring>("fsm_name");
	boost::shared_lock<boost::shared_mutex> lock(mutex_);
	auto find_itr = machines_.find(machine_name);
	if(find_itr != machines_.end())
	{
		find_itr->second->StopManagingObject(object);
	}
}