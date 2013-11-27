// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "building.h"
#include "swganh_core/messages/scene_create_object_by_crc.h"
#include "swganh/crc.h"

using namespace std;
using namespace swganh::object;

uint32_t Building::GetType() const
{
    return type;
}

void Building::CreateBaselines( std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
	auto dispatcher = GetEventDispatcher();
    if (dispatcher)
    {
		//need to write custom function as cells need to be send after building buio but before sendendbaseline
        //GetEventDispatcher()->Dispatch(make_shared<ObserverEvent>
          //                             ("Object::Baselines", shared_from_this(), observer));
    }
}

void Building::SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
   // auto lock = AcquireLock();
    //SendCreateByCrc(observer, lock);
}

void Building::SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer, boost::unique_lock<boost::mutex>& lock)
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