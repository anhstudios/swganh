// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "attributes_helper.h"
#include "swganh_core/object/tangible/tangible.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::messages;
using namespace swganh::attributes;

void AttributesHelper::SetOptionalAttribute(
    vector<Attribute>& attributes_,
    const string& attribute_name,
	const string& attribute_label,
    const shared_ptr<Object> object,
	bool recursive
)
{
	wstring attr_val;
	if (recursive)
		attr_val = object->GetAttributeRecursiveAsString(attribute_name);
	else
		attr_val = object->GetAttributeAsString(attribute_name);

	if (attr_val != L"")
	{
		attributes_.push_back(Attribute(attribute_label, attr_val));
	}
}

std::wstring AttributesHelper::GetCondition(const std::shared_ptr<swganh::object::Object> object)
{
	// condition is special
	auto tano = static_pointer_cast<Tangible>(object);
	std::wstring condition = object->GetAttributeAsString("condition");
	wstringstream ss;
	if (tano)
	{
		int32_t max_condition = tano->GetMaxCondition();
		ss << tano->GetCondition() << L"/" << max_condition;
		condition = ss.str();
		return condition;		
	}
	else
		return L"";
}
std::wstring AttributesHelper::GetVolume(const std::shared_ptr<swganh::object::Object> object)
{
	// volume is special
	auto tano = static_pointer_cast<Tangible>(object);
	std::wstring volume = object->GetAttributeAsString("volume");
	wstringstream ss;
	if (tano && volume.length() > 0)
	{
		object->SetAttribute<wstring>("volume", volume);
		return volume;		
	}
	else
		return volume;
}