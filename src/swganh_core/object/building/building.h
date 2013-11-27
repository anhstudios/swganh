// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "swganh_core/object/object.h"
#include "swganh_core/object/tangible/tangible.h"

namespace swganh
{
namespace object
{

class BuildingFactory;
class BuildingMessageBuilder;

class Building : public swganh::object::Tangible
{
public:
    typedef BuildingFactory FactoryType;
    typedef BuildingMessageBuilder MessageBuilderType;

    uint32_t GetType() const;
    const static uint32_t type = 0x4255494f;

	virtual void CreateBaselines( std::shared_ptr<swganh::observer::ObserverInterface> observer);

	/**
     * @brief Sends the create by crc message to the observer of 'this' object
	 * buildings currently have their own as special rules apply
     */
    
	virtual void SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer);
    virtual void SendCreateByCrc(std::shared_ptr<swganh::observer::ObserverInterface> observer, boost::unique_lock<boost::mutex>& lock);

private:

};

}
}  // namespace swganh::object
