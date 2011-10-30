
#ifndef SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_H_
#define SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_H_

#include <cstdint>
#include <string>

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace intangible {
    
class Intangible : public swganh::object::Object
{
public:
    // ITNO
    virtual uint32_t GetType() const { return Intangible::type; }
    const static uint32_t type = 0x494E534F;

    std::string GetStfDetailFile() ;
    void SetStfDetailFile(std::string stf_detail_file);

    std::string GetStfDetailString() ;
    void SetStfDetailString(std::string stf_detail_string);

    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();
private:
	friend class IntangibleMessageBuilder;
    friend class IntangibleFactory;
    std::string stf_detail_file_;
    std::string stf_detail_string_;
};
    
}}}  // namespace swganh::object::intangible

#endif  // SWGANH_OBJECT_BASE_OBJECT_H_
