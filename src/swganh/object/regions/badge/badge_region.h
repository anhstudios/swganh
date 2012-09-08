// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE


#include <swganh/object/object.h>

namespace swganh {
namespace object {
namespace regions {

class BadgeRegion : public swganh::object::Object
{
public:
	BadgeRegion(void);
	~BadgeRegion(void);

	void OnCollisionEnter(std::shared_ptr<swganh::object::Object> collider);

protected:
	void __BuildCollisionBox();
	void __BuildBoundingVolume();

private:

};

}}} // swganh::object::regions