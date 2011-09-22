
#include "swganh/simulation/scene.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::simulation;

Scene::Scene(const SceneDescription& description)
{}

Scene::Scene(string name, string label, string description, string terrain)
{}

void Scene::AddObject(const std::shared_ptr<swganh::object::Object>& object)
{}

void Scene::RemoveObject(const std::shared_ptr<swganh::object::Object>& object)
{}
