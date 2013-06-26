#pragma once

#include <memory>

namespace swganh
{
namespace object
{
	class Object;
}

namespace spawn
{
class FsmController;
class FiniteStateMachineInterface
{
public:
	virtual ~FiniteStateMachineInterface() {}

	virtual void StartManagingObject(std::shared_ptr<swganh::object::Object> object) = 0;
	virtual void StopManagingObject(std::shared_ptr<swganh::object::Object> object) = 0;

	virtual void MarkDirty(std::shared_ptr<FsmController> controller_) = 0;
};
}
}