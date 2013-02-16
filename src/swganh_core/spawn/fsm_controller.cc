#include "fsm_controller.h"

#include "swganh_core/object/object.h"
#include "swganh_core/spawn/fsm_bundle_interface.h"

using namespace swganh::spawn;

FsmController::FsmController(FiniteStateMachine* parent, std::shared_ptr<swganh::object::Object> object, 
							 std::shared_ptr<FsmBundleInterface> bundle)
	: parent_machine_(parent)
	, bundle_(bundle)
	, object_(object)
{
}

uint64_t FsmController::GetId() const
{
	return object_->GetObjectId();
}

void FsmController::Notify(swganh::messages::BaseSwgMessage* message)
{
	bundle_->HandleNotify(object_, message);
}

void FsmController::Cleanup(boost::posix_time::ptime current_time_)
{
	bundle_->HandleCleanup(object_, current_time_);
}

bool FsmController::IsDirty()
{
	return bundle_->IsDirty();
}