// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/intangible/intangible_factory.h"
#include <unordered_map>

namespace swganh {
namespace database {
class DatabaseManagerInterface;
}} // swganh::database

namespace sql {
class ResultSet;
}

namespace swganh {

namespace character{
	class CharacterProviderInterface;
}

namespace object {

    
    class Player;
    class PlayerFactory : public swganh::object::IntangibleFactory
    {
    public:
		typedef Player ObjectType;

        PlayerFactory(swganh::app::SwganhKernel* kernel);

        virtual void LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock);

        virtual uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited = false);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

		virtual void PersistChangedObjects();

        std::shared_ptr<swganh::object::Object> CreateObject();
        
        void RegisterEventHandlers();
    private:
		std::shared_ptr<swganh::character::CharacterProviderInterface> character_provider_;

		void PersistBadges_(const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void PersistXP_(const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void PersistDraftSchematics_(const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void PersistQuestJournal_(const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void PersistForceSensitiveQuests_(const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void PersistFriends_(const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void RemoveFriend_(const std::shared_ptr<Player>& player, uint64_t friend_id);
        void RemoveFromIgnoredList_(const std::shared_ptr<Player>& player, uint64_t ignore_player_id);
        void PersistIgnoredList_(const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void LoadStatusFlags_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void LoadProfileFlags_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void LoadBadges_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void LoadDraftSchematics_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void LoadFriends_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void LoadForceSensitiveQuests_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void LoadIgnoredList_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void LoadQuestJournal_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
        void LoadWaypoints_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
		void LoadXP_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock);
    };

}}  // namespace swganh::object
