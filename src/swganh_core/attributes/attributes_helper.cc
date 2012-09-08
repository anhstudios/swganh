// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "attributes_helper.h"
#include "swganh_core/object/object.h"

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