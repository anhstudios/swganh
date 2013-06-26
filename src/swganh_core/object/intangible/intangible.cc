// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "intangible.h"
#include "swganh_core/object/object_events.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;

Intangible::Intangible()
	: generic_int_(0)
{
}

uint32_t Intangible::GetGenericInt() {
    auto lock = AcquireLock();
    return GetGenericInt(lock);
}

uint32_t Intangible::GetGenericInt(boost::unique_lock<boost::mutex>& lock)
{
	return generic_int_;
}

void Intangible::SetGenericInt(uint32_t generic_int) {
    auto lock = AcquireLock();
    SetGenericInt(generic_int, lock);
}

void Intangible::SetGenericInt(uint32_t generic_int, boost::unique_lock<boost::mutex>& lock)
{
	generic_int_ = generic_int;
}

void Intangible::CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
	if (auto dispatch = GetEventDispatcher())
	{
		dispatch->Dispatch(make_shared<ObserverEvent>
			("Intangible::Baselines", shared_from_this(), observer));
	}
}
