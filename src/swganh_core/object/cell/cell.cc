// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "cell.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/scene_create_object_by_crc.h"
#include "swganh/crc.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

void Cell::SetCell(uint32_t cell_number)
{
    auto lock = AcquireLock();
    SetCell(cell_number, lock);
}

void Cell::SetCell(uint32_t cell_number, boost::unique_lock<boost::mutex>& lock)
{
    generic_int_ = cell_number;
    DISPATCH(Cell, Cell);
}

uint32_t Cell::GetCell()
{
    auto lock = AcquireLock();
    return GetCell(lock);
}

uint32_t Cell::GetCell(boost::unique_lock<boost::mutex>& lock)
{
    return generic_int_;
}

void Cell::CreateBaselines( std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
    auto dispatcher = GetEventDispatcher();
    if (dispatcher)
    {
		//need to write custom function as cells need to be send after building buio but before sendendbaseline
        //GetEventDispatcher()->Dispatch(make_shared<ObserverEvent>
          //                             ("Object::Baselines", shared_from_this(), observer));
    }
}

void Cell::SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
    //auto lock = AcquireLock();
    //SendCreateByCrc(observer, lock);
}

void Cell::SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer, boost::unique_lock<boost::mutex>& lock)
{
    //DLOG(info) << "SEND [" << GetObjectId() << "] (" << GetTemplate() <<") TO " << observer->GetId();
	/*
    swganh::messages::SceneCreateObjectByCrc scene_object;
    scene_object.object_id = GetObjectId(lock);
    scene_object.object_crc = swganh::memcrc(GetTemplate(lock));
    scene_object.position = GetPosition(lock);
    scene_object.orientation = GetOrientation(lock);
    scene_object.byte_flag = 0;
	
	
    observer->Notify(&scene_object);

    SendUpdateContainmentMessage(observer, lock, true);
	*/
}