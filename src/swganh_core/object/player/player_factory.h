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
namespace object {

    
    class Player;
    class PlayerFactory : public swganh::object::IntangibleFactory
    {
    public:
		typedef Player ObjectType;

        PlayerFactory(swganh::app::SwganhKernel* kernel);
        virtual uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object, bool persist_inherited = false);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);
		virtual void PersistChangedObjects();
        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObject();
        
        void RegisterEventHandlers();
    private:
        // Helpers
        void LoadStatusFlags_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
        void LoadProfileFlags_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
        void LoadBadges_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
		void PersistBadges_(const std::shared_ptr<Player>& player);
		void LoadXP_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
        void PersistXP_(const std::shared_ptr<Player>& player);
        void LoadWaypoints_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
        void PersistWaypoints_(const std::shared_ptr<Player>& player);
        void LoadDraftSchematics_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
        void PersistDraftSchematics_(const std::shared_ptr<Player>& player);
        void LoadQuestJournal_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
        void PersistQuestJournal_(const std::shared_ptr<Player>& player);
        void LoadForceSensitiveQuests_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
        void PersistForceSensitiveQuests_(const std::shared_ptr<Player>& player);
        void LoadFriends_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
        void PersistFriends_(const std::shared_ptr<Player>& player);
        void RemoveFriend_(const std::shared_ptr<Player>& player, uint64_t friend_id);
        void LoadIgnoredList_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
        void RemoveFromIgnoredList_(const std::shared_ptr<Player>& player, uint64_t ignore_player_id);
        void PersistIgnoredList_(const std::shared_ptr<Player>& player);
    };

}}  // namespace swganh::object
