#ifndef SWGANH_SUI_RADIAL_INTERFACE_H_
#define SWGANH_SUI_RADIAL_INTERFACE_H_

#include <memory>
#include <vector>

namespace swganh {
namespace object{
class Object;
}

namespace messages {
namespace controllers {
	struct RadialOptions;
}}

namespace sui {

class RadialInterface
{
public:
	virtual void BuildRadial(std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> target, std::vector<swganh::messages::controllers::RadialOptions> radials) = 0;
	virtual void HandleRadial(std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> target, uint8_t action) = 0;
};

}}

#endif
//  SWGANH_SUI_RADIAL_INTERFACE_H_