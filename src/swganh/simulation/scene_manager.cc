
#include "swganh/simulation/scene_manager.h"

#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "anh/logger.h"

#include "swganh/object/object.h"

#include "swganh/simulation/scene.h"

using namespace sql;
using namespace std;
using namespace swganh::object;
using namespace swganh::simulation;

void SceneManager::LoadSceneDescriptionsFromDatabase(const std::shared_ptr<sql::Connection>& connection)
{
    try 
    {
        auto statement = connection->createStatement();

        unique_ptr<ResultSet> result(statement->executeQuery(
            "CALL sp_GetSceneDescriptions()"));

        while(result->next())
        {
            SceneDescription description;
            description.id = result->getUInt("id");
            description.name = result->getString("name");
            description.label = result->getString("label");
            description.description = result->getString("description");
            description.terrain = result->getString("terrain");

            scene_descriptions_.insert(make_pair(description.label, description));
        }
    }
    catch(SQLException &e) 
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

std::shared_ptr<Scene> SceneManager::GetScene(const std::string& scene_label) const
{
    auto find_iter = scenes_.find(scene_label);

    if (find_iter == scenes_.end())
    {
        return nullptr;
    }

    return find_iter->second;
}

std::shared_ptr<Scene> SceneManager::GetScene(uint32_t scene_id) const
{
	auto find_iter = find_if(begin(scenes_), end(scenes_), [scene_id] (ScenePair scene_pair) {
		return scene_pair.second->GetSceneId() == scene_id;
	});

	if (find_iter == scenes_.end())
    {
        return nullptr;
    }
    
	return find_iter->second;
}

void SceneManager::StartScene(const std::string& scene_label)
{
	auto description_iter = scene_descriptions_.find(scene_label);

    if (description_iter == scene_descriptions_.end())
    {
        throw std::runtime_error("Requested an invalid scene: " + scene_label);
    }

    auto scene_iter = scenes_.find(scene_label);

    if (scene_iter != scenes_.end()) 
    {
        throw std::runtime_error("Scene has already been loaded: " + scene_label);
    }

    LOG(info) << "Starting scene: " << scene_label;

    auto scene = make_shared<Scene>(description_iter->second);

    scenes_.insert(make_pair(scene_label, scene));
}

void SceneManager::StopScene(const std::string& scene_label)
{
    scenes_.erase(scene_label);
}
