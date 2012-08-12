#ifndef SWGANH_SUI_ELEMENT_H_
#define SWGANH_SUI_PAGE_H_

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace swganh
{
namespace sui
{
class UIElement;

typedef std::map<std::string, std::string> UiPropertiesMap;
typedef std::vector<std::shared_ptr<UIElement>> SubElementsList;

class UIElement
{
public:

	const std::string& GetType() const;
	void SetType(const std::string& type);

	std::string GetProperty(const std::string& name) const;
	void SetProperty(const std::string& name, const std::string& value);

	void AddSubElement(std::shared_ptr<UIElement> sub_element);
	void RemoveSubElement(int index);
	const SubElementsList& GetSubElements() const;

private:
	std::string type_;
	UiPropertiesMap properties_;
	SubElementsList sub_elements_;
};

}
}

#endif