
#ifndef SWGANH_OBJECT_PLAYER_PLAYER_FACTORY_H_
#define SWGANH_OBJECT_PLAYER_PLAYER_FACTORY_H_

#include "swganh/object/object_factory.h"
#include <unordered_map>

namespace anh {
namespace database {
class DatabaseManagerInterface;
}} // anh::database

namespace sql {
class ResultSet;
}

namespace swganh {
namespace simulation {
    class SimulationService;
}}  // namespace swganh::simulation

namespace swganh {
namespace object {
namespace player {
    
    class Player;
    class PlayerFactory : public swganh::object::ObjectFactory
    {
    public:
        PlayerFactory(anh::database::DatabaseManagerInterface* db_manager,
            swganh::simulation::SimulationService* simulation_service,
            anh::EventDispatcher* event_dispatcher);

        void LoadTemplates();

        bool HasTemplate(const std::string& template_name);

        void PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name);

		virtual uint32_t GetType() const;
        const static uint32_t type;

        void RegisterEventHandlers();
    private:
        // Helpers
        void LoadStatusFlags_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
        void LoadProfileFlags_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement);
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

        std::unordered_map<std::string, std::shared_ptr<Player>>::iterator GetTemplateIter_(const std::string& template_name);
        std::unordered_map<std::string, std::shared_ptr<Player>> player_templates_;
    };

}}}  // namespace swganh::object::player

#endif  // SWGANH_OBJECT_PLAYER_PLAYER_FACTORY_H_
