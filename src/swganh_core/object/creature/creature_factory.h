// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/tangible/tangible_factory.h"
#include "swganh_core/static/skill_manager.h"
#include "swganh/event_dispatcher.h"

#include <unordered_map>

namespace swganh
{

namespace database
{
class DatabaseManagerInterface;
}
} // swganh::database

namespace sql
{
class Statement;
}  // namespace sql

namespace skill
{
class Skill;
}  // namespace skill we need to persist those

namespace swganh
{

namespace object
{

class Creature;
class CreatureFactory : public swganh::object::TangibleFactory
{
public:
    typedef Creature ObjectType;

    CreatureFactory(swganh::app::SwganhKernel* kernel);

    virtual void LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock);

    virtual uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited = false);
	
	 /**
     * @brief adds or removes a skill for a creature to the respective db table
	 * still to find out is how skill commands relate to that
     */
	virtual uint32_t PersistSkills(const std::shared_ptr<swganh::object::Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited = false);

    void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);
    virtual void PersistChangedObjects();
    virtual void RegisterEventHandlers();
    std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

    std::shared_ptr<swganh::object::Object> CreateObject();

private:

    void LoadSkills_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Creature>& creature,
                     boost::unique_lock<boost::mutex>& lock);

    void LoadSkillMods_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Creature>& creature,
                        boost::unique_lock<boost::mutex>& lock);

    void LoadSkillCommands_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Creature>& creature,
                            boost::unique_lock<boost::mutex>& lock);

    void LoadBuffs_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<swganh::object::Creature>& creature,
                    boost::unique_lock<boost::mutex>& lock);

};

}
}  // namespace swganh::object
