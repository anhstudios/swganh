
#include "swganh/object/weapon/weapon.h"

#include "swganh/messages/deltas_message.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object;
using namespace swganh::object::weapon;

Weapon::Weapon()
    : Tangible()
    , ukn1(3)
    , ukn2(0)
    , ukn3(0)
    , ukn4(5.0f)
    , ukn5(0)
    , ukn6(0)
    , ukn7(0)
    , ukn8(0)
{}

void Weapon::GetBaseline3()
{
    //auto message = CreateBaselinesMessage(Object::VIEW_3, 11);
    //
    //// base data
    //message.data.append(Tangible::GetBaseline3().get().data);
    //// ukns
    //message.data.write(ukn1);
    //message.data.write(ukn2);
    //message.data.write(ukn3);
    //message.data.write(ukn4);
    //message.data.write(ukn5);
    //message.data.write(ukn6);
    //message.data.write(ukn7);

    //return BaselinesMessage(move(message));
}