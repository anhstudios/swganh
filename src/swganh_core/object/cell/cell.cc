// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "cell.h"

#include "swganh_core/messages/deltas_message.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

void Cell::SetCell(uint32_t cell_number) {
    auto lock = AcquireLock();
    SetCell(cell_number, lock);
}

void Cell::SetCell(uint32_t cell_number, boost::unique_lock<boost::mutex>& lock) 
{
	generic_int_ = cell_number;
	DISPATCH(Cell, Cell);
}

uint32_t Cell::GetCell() {
    auto lock = AcquireLock();
    return GetCell(lock);
}

uint32_t Cell::GetCell(boost::unique_lock<boost::mutex>& lock)
{
	return generic_int_;
}
