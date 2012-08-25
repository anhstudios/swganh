// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>

#include <boost/thread/mutex.hpp>

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace intangible {

class IntangibleFactory;
class IntangibleMessageBuilder;
    
class Intangible : public swganh::object::Object
{
public:
    typedef IntangibleFactory FactoryType;
    typedef IntangibleMessageBuilder MessageBuilderType;

    // ITNO
    /**
     * @return The type of this object instance.
     */
    virtual uint32_t GetType() const { return Intangible::type; }
    const static uint32_t type = 0x494E534F;

    /**
     * @return The stf file containing the detailed description of the object.
     */
    std::string GetStfDetailFile();

    /**
     * @return The stf string containing the detailed description of the object.
     */
    std::string GetStfDetailString();

    /** 
     * Sets the stf string that contains the detailed description of the object.
     *
     * @param stf_file_name Stf file containing the string.
     * @param stf_string Name of the string containing the detailed description.
     */
    void SetStfDetail(const std::string& stf_file_name, const std::string& stf_string);
    
protected:
    virtual void GetBaseline6();

private:
    mutable boost::mutex intangible_mutex_;

    std::string stf_detail_file_;
    std::string stf_detail_string_;
};
    
}}}  // namespace swganh::object::intangible
