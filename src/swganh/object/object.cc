
#include "swganh/object/object.h"

#include "anh/observer/observer_interface.h"

using namespace anh::observer;
using namespace std;
using namespace swganh::object;


void Object::Subscribe(const shared_ptr<ObserverInterface<Object>>& observer)
{
    auto find_iter = std::find_if(
        observers_.begin(),
        observers_.end(),
        [&observer] (const std::shared_ptr<ObserverInterface<Object>>& stored_observer)
    {
        return observer == stored_observer;
    });

    if (find_iter != observers_.end())
    {
        return;
    }

    observers_.push_back(observer);
}

void Object::Unsubscribe(const shared_ptr<ObserverInterface<Object>>& observer)
{
    auto find_iter = std::find_if(
        observers_.begin(),
        observers_.end(),
        [&observer] (const std::shared_ptr<ObserverInterface<Object>>& stored_observer)
    {
        return observer == stored_observer;
    });
    
    if (find_iter == observers_.end())
    {
        return;
    }

    observers_.erase(find_iter);
}

void Object::NotifySubscribers(const Object& observable)
{
    for_each(
        observers_.begin(),
        observers_.end(),
        [&observable] (const std::shared_ptr<ObserverInterface<Object>>& stored_observer)
    {
        return stored_observer->Notify(observable);
    });
}
