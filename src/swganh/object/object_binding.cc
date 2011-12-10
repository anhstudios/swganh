
#include "swganh/object/object.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;

struct ObjectWrapper : Object, wrapper<Object>
{

};

BOOST_PYTHON_MODULE(swganh) {
    class_<ObjectWrapper, boost::noncopyable>("Object");
}
