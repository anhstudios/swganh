// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>

#include "swganh_core/object/object.h"

namespace swganh {
namespace object {


class IntangibleFactory;
class IntangibleMessageBuilder;
    
class Intangible : public swganh::object::Object
{
public:
    typedef IntangibleFactory FactoryType;
    typedef IntangibleMessageBuilder MessageBuilderType;

	Intangible();

    // ITNO
    /**
     * @return The type of this object instance.
     */
    virtual uint32_t GetType() const { return Intangible::type; }
    const static uint32_t type = 0x49544e4f;
	uint32_t GetGenericInt();
	void SetGenericInt(uint32_t generic_int);

	virtual std::shared_ptr<Object> Clone();
	void Clone(std::shared_ptr<Intangible> other);

    void CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer);
    
protected:
	std::atomic<uint32_t> generic_int_;
};
    
}}  // namespace swganh::object
