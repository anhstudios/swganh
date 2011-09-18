#ifndef SWGANH_OBJECT_CELL_H_
#define SWGANH_OBJECT_CELL_H_

#include "swganh/object/base_object.h"
#include <glm/glm.hpp>

namespace swganh {
namespace object {

class Cell : public BaseObject {

public:
    // SCLT
    virtual uint32_t GetType() { Cell::type; }
    const static uint32_t type = 0x53434c54;

    /**
    *   @brief Gets the current cell number
    *   @returns cell number
    */
    uint32_t GetCell() { return cell_number_; }
    /**
    *   @brief sets the current cell number
    *   @param cell number
    */
    void SetCell(uint32_t cell_number);
private:
    uint32_t cell_number_;			//update 3
};

}} // swganh::object


#endif //SWGANH_OBJECT_CELL_H_