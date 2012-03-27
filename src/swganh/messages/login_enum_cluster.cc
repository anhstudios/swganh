/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "swganh/messages/login_enum_cluster.h"
#include "swganh/login/login_client.h"

using namespace swganh::login;
using namespace swganh::messages;
using namespace std;

LoginEnumCluster swganh::messages::BuildLoginEnumCluster(shared_ptr<LoginClient> login_client, const vector<GalaxyStatus>& galaxy_status) {
	LoginEnumCluster message;
	message.max_account_chars = 2;
	
	std::for_each(galaxy_status.begin(), galaxy_status.end(), [&message] (const GalaxyStatus& status) {
		Cluster cluster;
		cluster.server_id = status.galaxy_id;
		cluster.server_name = status.name;
		cluster.distance = status.distance;
		
		message.servers.push_back(cluster);
	});
	
	return message;
}