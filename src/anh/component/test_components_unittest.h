/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBANH_COMPONENT_TEST_COMPONENTS_H_
#define LIBANH_COMPONENT_TEST_COMPONENTS_H_

#include <anh/component/base_component.h>
#include <anh/component/component_loader_interface.h>

using namespace anh::component;

namespace anh { 
namespace test_components {

//================================================================
// Transform Component
//================================================================

struct Vector3
{
	Vector3(float _x = 0, float _y = 0, float _z = 0)
		: x(_x)
		, y(_y)
		, z(_z) { }

	float x, y, z;
};

struct Quaternion
{
	Quaternion(float _x = 0, float _y = 0, float _z = 0, float _w = 0)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w) { }

	float x, y, z, w;
};

class NullTransformComponent;

class TransformComponentInterface : public BaseComponent
{
public:
	TransformComponentInterface(const ComponentType& concrete_type) : BaseComponent("Transform", concrete_type) { }

	virtual Vector3& position() = 0;
	virtual Quaternion& rotation() = 0;

	static std::shared_ptr<NullTransformComponent> NullComponent;
};

class NullTransformComponent : public TransformComponentInterface
{
public:
	NullTransformComponent() : TransformComponentInterface("Anh.NullTransform") { }

	Vector3& position() { return position_; }
	Quaternion& rotation() { return rotation_; }

private:
	Vector3 position_;
	Quaternion rotation_;
};

class TransformComponent : public TransformComponentInterface
{
public:
	TransformComponent() : TransformComponentInterface("Anh.Transform") { }

	void Init(boost::property_tree::ptree& pt) {
		position_.x = pt.get<float>("position.x", 0.0f);
		position_.y = pt.get<float>("position.y", 0.0f);
		position_.z = pt.get<float>("position.z", 0.0f);
		rotation_.x = pt.get<float>("rotation.x", 0.0f);
		rotation_.y = pt.get<float>("rotation.y", 0.0f);
		rotation_.z = pt.get<float>("rotation.z", 0.0f);
		rotation_.w = pt.get<float>("rotation.w", 1.0f);
	}

	Vector3& position() { return position_; }
	Quaternion& rotation() { return rotation_; }

private:
	Vector3 position_;
	Quaternion rotation_;
};

class TransformComponentLoader : public ComponentLoaderInterface
{
public:
	bool Load(std::shared_ptr<ComponentInterface> comp) {
		std::shared_ptr<TransformComponentInterface> component = std::dynamic_pointer_cast<TransformComponentInterface>(comp);
		component->position().x = 100;
		component->position().y = 48;
		component->position().z = 3485;

		return true;
	}

	void Unload(std::shared_ptr<ComponentInterface> comp) {
	}
};

//================================================================
// Appearance Component
//================================================================
struct Appearance {
    Appearance(std::string _iff = "", std::string _client = "",std::string _appearance= "", std::string _animation= "", std::string _movement= "", std::string _name= "", std::string _detail= "", float _size = 1, uint32_t _crc = 0xDEADBEEF)
    : iff(_iff)
    , client_file(_client)
    , appearance_file(_appearance)
    , movement_file(_movement)
    , name(_name)
    , detail_desc(_detail)
    , size(_size)
    , crc(_crc) {}

    std::string iff, client_file, appearance_file, animation_file, movement_file, name, detail_desc;
    float& size;
    uint32_t& crc;
};
class NullAppearanceComponent;

class AppearanceComponentInterface : public BaseComponent
{
public:
	AppearanceComponentInterface(const ComponentType& concrete_type) : BaseComponent("Appearance", concrete_type) { }
    virtual Appearance& appearance() =0;
	static std::shared_ptr<NullAppearanceComponent> NullComponent;
};

class NullAppearanceComponent : public AppearanceComponentInterface
{
public:
    NullAppearanceComponent() : AppearanceComponentInterface("Anh.NullAppearance") { }

    virtual std::string& iff() { return appearance_.iff; }
    virtual std::string& client_file() { return appearance_.client_file; }
    virtual std::string& appearance_file()  { return appearance_.appearance_file; }
    virtual std::string& animation_file() { return appearance_.animation_file; }
    virtual std::string& movement_file()  { return appearance_.movement_file; }
    virtual std::string& name() { return appearance_.name; }
    virtual std::string& detail_desc() { return appearance_.detail_desc; }
    virtual float& size() { return appearance_.size; }
    virtual uint32_t& crc() { return appearance_.crc; }

    virtual Appearance& appearance() { return appearance_; }
private:
    Appearance appearance_;
};

class AppearanceComponent : public AppearanceComponentInterface
{    
public:
	AppearanceComponent() : AppearanceComponentInterface("Anh.Appearance") { }

	void Init(boost::property_tree::ptree& pt) {
        appearance_.iff = pt.get<char>("appearance.iff", "object/mobile/tatooine_npc/shared_windom_starkiller.iff");
		appearance_.client_file = pt.get<char>("appearance.client_file", "clientdata/npc/client_shared_npc_human_m.cdf");
		appearance_.appearance_file = pt.get<char>("appearance.appearance_file", "appearance/hum_m.sat");
        appearance_.animation_file = pt.get<char>("appearance.animation_file","all_male.map");
		appearance_.movement_file = pt.get<char>("appearance.movement_file", "datatables/movement/movement_human.iff");
		appearance_.name = pt.get<char>("appearance.description", "@npc_detail:human_base_male");
		appearance_.detail_desc = pt.get<char>("appearance.detail_desc", "@npc_detail:human_base_male");
        appearance_.size = pt.get<float>("appearance.size", 1.0);
        appearance_.crc = pt.get<uint32_t>("appearance.crc", 0xDEADBEEF);
	}
    Appearance& appearance() { return appearance_; }

private:
	Appearance appearance_;
};
class AppearanceComponentLoader : public ComponentLoaderInterface
{
public:
	bool Load(std::shared_ptr<ComponentInterface> comp) {
		std::shared_ptr<AppearanceComponentInterface> component = std::dynamic_pointer_cast<AppearanceComponentInterface>(comp);
        component->appearance().name = "test";
        component->appearance().crc = 0xDEEDBEE;
		component->appearance().size = 1.5;

		return true;
	}

	void Unload(std::shared_ptr<ComponentInterface> comp) {
	}
};

//================================================================
// Radial Component
//================================================================
class RadialComponentInterface : public BaseComponent
{
public:
    RadialComponentInterface(const ComponentType& concrete_type)
	: BaseComponent("Radial", concrete_type) { }
};

class NullRadialComponent : public RadialComponentInterface
{
public:
	NullRadialComponent() : RadialComponentInterface("Anh.NullRadial") { }
};

class RadialComponent : public RadialComponentInterface
{
public:
	RadialComponent() : RadialComponentInterface("Anh.Radial") { }
};

//================================================================
// Tickable Component
//================================================================

class NullTickableComponent;

class TickableComponentInterface : public BaseComponent
{
public:
	TickableComponentInterface(const ComponentType& concrete_type) : BaseComponent("Tickable", concrete_type) { }

	virtual bool ticked() = 0;
	static std::shared_ptr<NullTickableComponent> NullComponent;
};

class NullTickableComponent : public TickableComponentInterface
{
public:
	NullTickableComponent() : TickableComponentInterface("Anh.NullTickable") { }

	bool ticked() { return false; }

private:
};

class TickableComponent : public TickableComponentInterface
{
public:
	TickableComponent()
		: TickableComponentInterface("Anh.Tickable")
		, ticked_(false) { }

	bool ticked() { return ticked_; }
	void Update(const float timeout) { ticked_ = true; }

private:
	bool ticked_;
};

//================================================================
// HAM Component
//================================================================

class DoDamageMessage : public SimpleMessage
{
public:
	DoDamageMessage(uint8_t bar, uint32_t amount)
		: SimpleMessage(MessageType("DoDamage"), 0)
		, bar_(bar)
		, amount_(amount) { }

	const uint8_t bar() const { return bar_; }
	const uint32_t amount() const { return amount_; }

private:
	uint8_t bar_;
	uint32_t amount_;
};

struct HAM
{
public:
    HAM() { health, max_health, action, max_action, mind, max_mind = 0; }
	unsigned int health;
	unsigned int max_health;
	unsigned int action;
	unsigned int max_action;
	unsigned int mind;
	unsigned int max_mind;
};

class NullHAMComponent;

class HAMComponentInterface : public BaseComponent
{
public:
	HAMComponentInterface(const ComponentType& concrete_type) : BaseComponent("Ham", concrete_type) { }
    /*void Init(boost::property_tree::ptree pt)
	{
		ham_.max_health = pt.get<unsigned int>("health", 1);
		ham_.health = pt.get<unsigned int>("health", 1);
		ham_.max_action = pt.get<unsigned int>("action", 1);
		ham_.action = pt.get<unsigned int>("action", 1);
		ham_.max_mind = pt.get<unsigned int>("mind", 1);
		ham_.mind = pt.get<unsigned int>("mind", 1);
	}*/

    virtual HAM& ham() { return ham_; }

	static std::shared_ptr<NullHAMComponent> NullComponent;
private:
    HAM ham_;
};

class NullHAMComponent : public HAMComponentInterface
{
public:
	NullHAMComponent() : HAMComponentInterface("Anh.NullHam") { }

	HAM& ham() { return ham_; }

private:
	HAM ham_;
};

class HAMComponent : public HAMComponentInterface
{
public:
	HAMComponent() : HAMComponentInterface("Anh.Ham")
	{ 
		RegisterMessageHandler(MessageType("DoDamage"), std::bind(&HAMComponent::handleDoDamage, this, std::placeholders::_1));
	}

	~HAMComponent()
	{
		UnregisterMessageHandler(MessageType("DoDamage"));
	}

	void Init(boost::property_tree::ptree pt)
	{
		ham_.max_health = pt.get<unsigned int>("health", 1);
		ham_.health = pt.get<unsigned int>("health", 1);
		ham_.max_action = pt.get<unsigned int>("action", 1);
		ham_.action = pt.get<unsigned int>("action", 1);
		ham_.max_mind = pt.get<unsigned int>("mind", 1);
		ham_.mind = pt.get<unsigned int>("mind", 1);
	}

	HAM& ham() { return ham_; }

private:
	HAM ham_;

	bool handleDoDamage(const Message m)
	{
		std::shared_ptr<DoDamageMessage> message = std::dynamic_pointer_cast<DoDamageMessage>(m);

		switch(message->bar())
		{
		case 1:
			ham_.health -= message->amount();
			break;

		case 2:
			ham_.action -= message->amount();
			break;

		case 3:
			ham_.mind -= message->amount();
			break;
		}

		return true;
	}
};

} // namespace test_components
} // namespace anh

#endif // LIBANH_COMPONENT_TEST_COMPONENTS_H_