#include <anh/component/component_interface.h>
#include <anh/component/base_component.h>


class DoDamageMessage : public anh::component::SimpleMessage
{
public:
	DoDamageMessage(uint8_t bar, uint32_t amount)
		: anh::component::SimpleMessage(anh::component::MessageType("DoDamage"), 0)
		, bar_(bar)
		, amount_(amount) { }

	const uint8_t bar() const { return bar_; }
	const uint32_t amount() const { return amount_; }

private:
	uint8_t bar_;
	uint32_t amount_;
};
class HAMStat
{
public:
    HAMStat();

    // get/setters
    void        base_hitpoints(uint32_t value) { base_hitpoints_ = value; }
    uint32_t    base_hitpoints() { return base_hitpoints_; }
    void        current_hitpoints(uint32_t value) { current_hitpoints_ = value; }
    uint32_t    current_hitpoints() { return current_hitpoints_; }
    void        max_hitpoints(uint32_t value) { max_hitpoints_ = value; }
    uint32_t    max_hitpoints() { return max_hitpoints_; }
    void        encumbrance(uint32_t value) { encumbrance_ = value; }
    uint32_t    encumbrance() { return encumbrance_; }
    void        modified_hitpoints(uint32_t value) { modified_hitpoints_ = value; }
    uint32_t    modified_hitpoints() { return modified_hitpoints_; }
    void        modifier(uint32_t value) { modifier_ = value; }
    uint32_t    modifier() { return modifier_; }
    void        wounds(uint32_t value) { wounds_ = value; }
    uint32_t    wounds() { return wounds_; }
    void        regen_rate(float value) { regen_rate_ = value; }
    float       regen_rate() { return regen_rate_; }
private:
    uint32_t    base_hitpoints_;        // normal hitpoints without other factors
    uint32_t    current_hitpoints_;     // current available hitpoints
    uint32_t    max_hitpoints_;         // max available hitpoints
    uint32_t    encumbrance_;           //
    uint32_t    modified_hitpoints_;    // max hit points after BF and wounds
    uint32_t    modifier_;              // buffs
    uint32_t    wounds_;                // wounds that decrease the amount of possible hit point value
    float       regen_rate_;            // the regeneration rate
};

class HAM
{
public:
    HAM();
	HAMStat health, strength, constitution;
	HAMStat action, max_action, quickness, max_quickness, stamina, max_stamina;
	HAMStat mind, max_mind, focus, max_focus, willpower, max_willpower;
};

class NullHAMComponent;

class HAMComponentInterface : public anh::component::BaseComponent
{
public:
	HAMComponentInterface(const anh::component::ObjectId& id)
		: BaseComponent(id) { }
    
    void Init(boost::property_tree::ptree pt);
    virtual HAM& ham() { return ham_; }

	static std::shared_ptr<NullHAMComponent> NullComponent;
private:
    HAM ham_;
};

class NullHAMComponent : public HAMComponentInterface
{
public:
	NullHAMComponent()
		: HAMComponentInterface(0) { }

	HAM& ham() { return ham_; }
	const anh::component::ComponentInfo& component_info() { return component_info_; }

private:
	HAM ham_;
	static anh::component::ComponentInfo component_info_;
};

class HAMComponent : public HAMComponentInterface
{
public:
	HAMComponent(const anh::component::ObjectId& id)
		: HAMComponentInterface(id) 
	{ 
		RegisterMessageHandler(anh::component::MessageType("DoDamage"), std::bind(&HAMComponent::handleDoDamage, this, std::placeholders::_1));
	}

	~HAMComponent()
	{
		UnregisterMessageHandler(anh::component::MessageType("DoDamage"));
	}

	void Init(boost::property_tree::ptree pt);

	HAM& ham() { return ham_; }
	const anh::component::ComponentInfo& component_info() { return component_info_; }

private:
	HAM ham_;
	static anh::component::ComponentInfo component_info_;

	bool handleDoDamage(const anh::component::Message m)
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