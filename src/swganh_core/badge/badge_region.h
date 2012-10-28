// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE


#include <swganh_core/object/object.h>

namespace swganh {
namespace badge {
	class BadgeService;
}
namespace object {
namespace regions {

class BadgeRegion : public swganh::object::Object
{
public:
	BadgeRegion(std::string badge_name, swganh::badge::BadgeService* service);
	~BadgeRegion(void);

	void OnCollisionEnter(std::shared_ptr<swganh::object::Object> collider);

protected:
	void __BuildCollisionBox();

private:
	swganh::badge::BadgeService* badge_service_;
	std::string badge_name_;
};

}}} // swganh::object::regions