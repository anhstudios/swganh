
#ifndef SWGANH_OBJECT_MANUFACTURE_SCHEMATIC_H_
#define SWGANH_OBJECT_MANUFACTURE_SCHEMATIC_H_

#include "swganh/object/base_object.h"

namespace swganh {
namespace object {
namespace intangible {

class ManufactureSchematic : public BaseObject
{
public:
    struct Property
    {
        std::string property_stf_file;
        std::string property_stf_name;
        float value;
    };

private:
    uint32_t schematic_quantity_;
    std::vector<Property> properties_;
    std::wstring creator_;
    uint32_t complexity_;
    float schematic_data_size_;
};

}}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_MANUFACTURE_SCHEMATIC_H_
