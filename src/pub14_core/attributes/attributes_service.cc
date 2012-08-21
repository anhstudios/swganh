// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "attributes_service.h"

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include "anh/logger.h"

#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

using swganh::app::SwganhKernel;
using namespace swganh::app;
using namespace swganh_core::attributes;

#ifdef WIN32
using std::wregex;
using std::wsmatch;
using std::regex_match;
#else
using boost::wregex;
using boost::wsmatch;
using boost::regex_match;
#endif

AttributesService::AttributesService(SwganhKernel* kernel)
    : kernel_(kernel)
{}

AttributesService::~AttributesService()
{    
	attribute_templates_.clear();
}

std::shared_ptr<swganh::attributes::AttributeTemplateInterface> AttributesService::GetAttributeTemplate(const std::string& name)
{

}
void AttributesService::SetAttributeTemplate(const std::shared_ptr<swganh::attributes::AttributeTemplateInterface> template_, const std::string& name)
{

}

