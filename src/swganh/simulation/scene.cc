
#include "swganh/simulation/scene.h"

#include <algorithm>

#include "swganh/object/object.h"
#include "swganh/object/object_controller.h"
#include "swganh/messages/scene_destroy_object.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object;
using namespace swganh::simulation;

class Scene::SceneImpl
{
public:
    SceneImpl(SceneDescription description)
        : description_(move(description))
    {}

    const SceneDescription& GetDescription() const
    {
        return description_;
    }

    void AddObject(const shared_ptr<Object>& object)
    {
		InsertObject(object);

        for_each(begin(object_map_), end(object_map_),
            [&object] (const ObjectMap::value_type& object_entry) 
        {
            auto& stored_object = object_entry.second;

            stored_object->AddAwareObject(object);
            object->AddAwareObject(stored_object);
        });

        auto contained_objects = object->GetContainedObjects();
        
        for_each(begin(contained_objects), end(contained_objects),
            [this] (const ObjectMap::value_type& object_entry) 
        {
            auto& stored_object = object_entry.second;
            AddObject(stored_object);
        });

    }
    
    void RemoveObject(const shared_ptr<Object>& object)
    {
		EraseObject(object);
                
        SceneDestroyObject destroy_message;
        destroy_message.object_id = object->GetObjectId();

        for_each(begin(object_map_), end(object_map_),
            [&object, &destroy_message] (const ObjectMap::value_type& object_entry) 
        {
            auto& stored_object = object_entry.second;
            
            stored_object->RemoveContainedObject(object);
            stored_object->RemoveAwareObject(object);

            if (stored_object->HasController())
            {
                stored_object->GetController()->Notify(destroy_message);
            }
        });
    }

	void InsertObject(const shared_ptr<Object>& object)
	{
		// make sure it's not already there
		auto find_iter = objects_.find(object);
		if (find_iter == end(objects_))
			objects_.insert(find_iter, object);

		auto find_map = object_map_.find(object->GetObjectId());
		if (find_map == end(object_map_))
			object_map_.insert(find_map, ObjectPair(object->GetObjectId(), object));
	}

	void EraseObject(const shared_ptr<Object>& object)
	{        
		objects_.erase(object);
        object_map_.erase(object->GetObjectId());
	}


private:

    typedef std::map<
        uint64_t,
        shared_ptr<Object>
    > ObjectMap;
	typedef std::pair<
		uint64_t,
		shared_ptr<Object>
	> ObjectPair;

    typedef std::set<std::shared_ptr<Object>> ObjectSet;

    ObjectSet objects_;
    ObjectMap object_map_;

    SceneDescription description_;
};

Scene::Scene(SceneDescription description)
: impl_(new SceneImpl(move(description)))
{}

Scene::Scene(uint32_t scene_id, string name, string label, string description, string terrain) 
{
    SceneDescription scene_description;

    scene_description.id = scene_id;
    scene_description.name = move(name);
    scene_description.label = move(label);
    scene_description.description = move(description);
    scene_description.terrain = move(terrain);

    impl_.reset(new SceneImpl(move(scene_description)));
}

uint32_t Scene::GetSceneId() const
{
    return impl_->GetDescription().id;
}

const std::string& Scene::GetName() const
{
    return impl_->GetDescription().name;
}

const std::string& Scene::GetLabel() const
{
    return impl_->GetDescription().label;
}

const std::string& Scene::GetDescription() const
{
    return impl_->GetDescription().description;
}
const std::string& Scene::GetTerrainMap() const 
{
	return impl_->GetDescription().terrain;
}
void Scene::AddObject(const std::shared_ptr<swganh::object::Object>& object)
{
    impl_->AddObject(object);
}

void Scene::RemoveObject(const std::shared_ptr<swganh::object::Object>& object)
{
    impl_->RemoveObject(object);
}
