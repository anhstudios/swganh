#include "npc_neutral_bundle.h"

using namespace swganh::spawn;
using namespace swganh::messages;
using namespace swganh::object;
using namespace boost::posix_time;
using namespace std;

NpcNeutralBundle::NpcNeutralBundle(shared_ptr<FsmStateInterface> initial_state)
{
}

void NpcNeutralBundle::HandleNotify(shared_ptr<Object>& object_, BaseSwgMessage* message)
{
}

void NpcNeutralBundle::HandleCleanup(shared_ptr<Object>& object_, ptime current_time_)
{
}

bool NpcNeutralBundle::IsDirty()
{
	return false;
}

void NpcNeutralBundle::SetCurrentState(std::shared_ptr<FsmStateInterface> new_state, ptime current_time)
{
}