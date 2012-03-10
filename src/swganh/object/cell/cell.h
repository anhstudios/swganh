#ifndef SWGANH_OBJECT_CELL_CELL_H_
#define SWGANH_OBJECT_CELL_CELL_H_

#include <glm/glm.hpp>

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace cell {

class Cell : public swganh::object::Object {

public:
    // SCLT
    virtual uint32_t GetType() const { return Cell::type; }
    const static uint32_t type = 0x53434c54;

    /**
    * @brief Gets the current cell number
    *   @returns cell number
    */
    uint32_t GetCell() { return cell_number_; }
    /**
    * @brief sets the current cell number
    *   @param cell number
    */
    void SetCell(uint32_t cell_number);

    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();

private:
    uint32_t cell_number_;			//update 3
};

}}} // swganh::object::cell

#endif //SWGANH_OBJECT_CELL_CELL_H_
