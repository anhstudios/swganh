#include "ui_element.h"

using namespace swganh::sui;

const std::string& UIElement::GetType() const
{
	return type_;
}

void UIElement::SetType(const std::string& type)
{
	type_ = type;
}

std::string UIElement::GetProperty(const std::string& name) const
{
	auto search = properties_.find(name);
	if(search != properties_.end())
	{
		return search->second;
	}
	else
	{
		return "";
	}
}

void UIElement::SetProperty(const std::string& name, const std::string& value)
{
	properties_.insert(UiPropertiesMap::value_type(name, value));
}

void UIElement::AddSubElement(std::shared_ptr<UIElement> sub_element)
{
	sub_elements_.push_back(sub_element);
}

void UIElement::RemoveSubElement(int index)
{
	sub_elements_.erase(sub_elements_.begin() + index);
}

const SubElementsList& UIElement::GetSubElements() const
{
	return sub_elements_;
}