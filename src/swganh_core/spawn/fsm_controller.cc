#include "fsm_controller.h"

FsmController::FsmController(FiniteStateMachine* parent, std::shared_ptr<swganh::object::Object> object, 
							 std::shared_ptr<FsmBundleInterface> bundle)
{
}

uint64_t FsmController::GetId() const
{
	return object_->GetObjectId();
}

void FsmController::Notify(swganh::messages::BaseSwgMessage* message)
{
	
}

void FsmController::Cleanup(boost::posix_time::ptime current_time_)
{
}

bool FsmController::IsDirty()
{
}