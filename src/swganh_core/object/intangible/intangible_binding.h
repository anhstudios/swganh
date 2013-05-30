// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include "swganh_core/object/intangible/intangible.h"

#include <boost/python.hpp>

void exportIntangible()
{
    namespace bp = boost::python;
    namespace so = swganh::object;

    bp::class_<so::Intangible, bp::bases<so::Object>, std::shared_ptr<so::Intangible>, boost::noncopyable>("Intangible")
    ;

    bp::implicitly_convertible<std::shared_ptr<so::Intangible>, std::shared_ptr<so::Object>>();
	bp::implicitly_convertible<std::shared_ptr<so::Intangible>, std::shared_ptr<so::ContainerInterface>>();
}
