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

uint32_t Intangible::GetGenericInt()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	return generic_int_;
}

void Intangible::SetGenericInt(uint32_t generic_int)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		generic_int_ = generic_int;
	}

}

std::shared_ptr<Object> Intangible::Clone()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	auto other = make_shared<Intangible>();
	Clone(other);
	return other;
}

void Intangible::Clone(std::shared_ptr<Intangible> other)
{
	other->generic_int_.store(generic_int_);

	//Call the method in the super class
	Object::Clone(other);
}

void Intangible::CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
	if (event_dispatcher_)
	{
		GetEventDispatcher()->Dispatch(make_shared<ObserverEvent>
			("Intangible::Baselines", shared_from_this(), observer));
	}
}
