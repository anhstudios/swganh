#ifndef SWGANH_OBJECT_INTAGIBLE_CELL_H_
#define SWGANH_OBJECT_INTAGIBLE_CELL_H_

#include "swganh/object/intangible.h"
#include <glm\glm.hpp>

namespace swganh {
namespace object {
namespace intangible {

class Cell : public swganh::object::Intangible {

public:
    // SCLT
    virtual uint32_t GetType() { Cell::type; }
    const static uint32_t type = 0x53434c54;

protected:
    virtual void OnReliableUpdate() {
        Intangible::OnReliableUpdate();
    }
private:
    uint32_t cell_number;			//update 3
};

}}} // swganh::object::intangible


#endif //SWGANH_OBJECT_INTAGIBLE_CELL_H_