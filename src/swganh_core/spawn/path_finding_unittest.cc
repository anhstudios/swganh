#include "fsm_state_interface.h"
#include "swganh_core/object/object.h"

#include <boost/test/unit_test.hpp>

using namespace swganh::object;
using namespace swganh::spawn;

/*
class PathFindingTest
{
public:
	PathFindingTest()
		: event_dispatcher_(io_service_) 
	{ 
		object_ = make_object(); 
	}

	std::shared_ptr<Object> make_object()
	{
		std::shared_ptr<Object> temp = std::make_shared<Object>();
		temp->SetEventDispatcher(&event_dispatcher_);
		return temp;
	}

protected:
	virtual void SetUp() { }

	std::shared_ptr<Object> object_;
	anh::EventDispatcher event_dispatcher_;
	boost::asio::io_service io_service_;
};

BOOST_FIXTURE_TEST_SUITE(PathingSuite, PathFindingTest)

BOOST_AUTO_TEST_CASE(canFindTrivialPath)
{
	try {
	std::vector<glm::vec3> path1 = FsmStateInterface::FindPath(object_, glm::vec3(1.0, 0.0, 0.0));
	std::vector<glm::vec3> path2 = FsmStateInterface::FindPath(object_, glm::vec3(0.0, 1.0, 0.0));
	} 
	catch(...)
	{
	}

	
	BOOST_CHECK_EQUAL(path1.size(), 5);
	BOOST_CHECK_EQUAL(path2.size(), 5);
	
}

BOOST_AUTO_TEST_SUITE_END()

	*/