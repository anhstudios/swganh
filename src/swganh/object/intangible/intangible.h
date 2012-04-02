
#ifndef SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_H_
#define SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_H_

#include <cstdint>
#include <mutex>
#include <string>

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace intangible {
    
class Intangible : public swganh::object::Object
{
public:
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
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();

private:
    mutable std::mutex intangible_mutex_;

    friend class IntangibleFactory;
    std::string stf_detail_file_;
    std::string stf_detail_string_;
};
    
}}}  // namespace swganh::object::intangible

#endif  // SWGANH_OBJECT_BASE_OBJECT_H_
