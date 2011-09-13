
#ifndef SWGANH_OBJECT_INTANGIBLE_H_
#define SWGANH_OBJECT_INTANGIBLE_H_

#include <cstdint>
#include <string>

#include "swganh/object/base_object.h"

namespace swganh {
namespace object {
    
class Intangible : public BaseObject
{
public:
    // ITNO
    virtual uint32_t GetType() { return Intangible::type; }
    const static uint32_t type = 0x49544e4f;

    std::string GetStfDetailFile() const;
    void SetStfDetailFile(std::string stf_detail_file);

    std::string GetStfDetailString() const;
    void SetStfDetailString(std::string stf_detail_string);

protected:
    virtual void OnReliableUpdate() {}

private:
    std::string stf_detail_file_;
    std::string stf_detail_string_;
};
    
}}  // namespace

#endif  // SWGANH_OBJECT_BASE_OBJECT_H_
