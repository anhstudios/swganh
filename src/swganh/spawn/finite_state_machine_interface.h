#pragma once

namespace swganh
{
namespace object
{
	class Object;
}

namespace spawn
{
class FsmControllerInterface;
class FiniteStateMachineInterface
{
public:
	void StartManagingObject(std::shared_ptr<swganh::object::Object> object);
	void StopManagingObject(std::shared_ptr<swganh::object::Object> object);

	void MarkDirty(std::shared_ptr<FsmControllerInterface> controller_);
};
}
}