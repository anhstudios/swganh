// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "cell.h"

#include "swganh_core/messages/deltas_message.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

void Cell::SetCell(uint32_t cell_number) {
	generic_int_ = cell_number;
	GetEventDispatcher()->Dispatch(make_shared<ObjectEvent>
		("Cell::Cell",shared_from_this()));

}

uint32_t Cell::GetCell()
{
	return generic_int_;
}


std::shared_ptr<Object> Cell::Clone()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	auto other = make_shared<Cell>();
	Clone(other);
	return other;
}

void Cell::Clone(std::shared_ptr<Cell> other)
{
	Intangible::Clone(other);
}