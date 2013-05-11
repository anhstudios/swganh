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

        virtual void LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object);

        virtual uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object, bool persist_inherited = false);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

		virtual void PersistChangedObjects();

        std::shared_ptr<swganh::object::Object> CreateObject();
        
        void RegisterEventHandlers();
    private:
		void PersistBadges_(const std::shared_ptr<Player>& player);
        void PersistXP_(const std::shared_ptr<Player>& player);
        void PersistWaypoints_(const std::shared_ptr<Player>& player);
        void PersistDraftSchematics_(const std::shared_ptr<Player>& player);
        void PersistQuestJournal_(const std::shared_ptr<Player>& player);
        void PersistForceSensitiveQuests_(const std::shared_ptr<Player>& player);
        void PersistFriends_(const std::shared_ptr<Player>& player);
        void RemoveFriend_(const std::shared_ptr<Player>& player, uint64_t friend_id);
        void RemoveFromIgnoredList_(const std::shared_ptr<Player>& player, uint64_t ignore_player_id);
        void PersistIgnoredList_(const std::shared_ptr<Player>& player);
        
        void LoadStatusFlags_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player);
        void LoadProfileFlags_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player);
        void LoadBadges_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player);
        void LoadDraftSchematics_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player);
        void LoadFriends_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player);
        void LoadForceSensitiveQuests_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player);
        void LoadIgnoredList_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player);
        void LoadQuestJournal_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player);
        void LoadWaypoints_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player);
		void LoadXP_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player);
    };

}}  // namespace swganh::object
