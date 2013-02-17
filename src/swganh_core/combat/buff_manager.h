// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <map>
#include <set>
#include <vector>
#include <queue>

#include <boost/thread/mutex.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <swganh_core/combat/buff_interface.h>
#include <swganh/app/swganh_kernel.h>

namespace swganh
{
namespace combat
{
	typedef std::pair<boost::posix_time::ptime, std::shared_ptr<swganh::object::Creature>> BuffQueueType;

	struct timeOrder
	{
		bool operator()(const BuffQueueType& lhs, const BuffQueueType& rhs) const
		{
			return lhs.first < rhs.first;
		}
	};

	typedef std::priority_queue<BuffQueueType, std::vector<BuffQueueType>, timeOrder> BuffQueue;

	class BuffManager
	{
	public:

		BuffManager(swganh::app::SwganhKernel* kernel);

		void Start();

	private:
		void BuffManager::handleTick_(const boost::system::error_code& e);

		swganh::app::SwganhKernel * kernel_;

		boost::mutex mutex_;

		BuffQueue q_;
		std::map<swganh::HashString, std::shared_ptr<BuffInterface>> buffs_;
		boost::asio::deadline_timer timer_;
	};

}
}