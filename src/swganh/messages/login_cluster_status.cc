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

#include "swganh/messages/login_cluster_status.h"

using namespace swganh::login;
using namespace swganh::messages;
using namespace std;

LoginClusterStatus swganh::messages::BuildLoginClusterStatus(const vector<GalaxyStatus>& galaxy_status) {
	LoginClusterStatus message;
	
    std::for_each(galaxy_status.begin(), galaxy_status.end(), [&message] (const GalaxyStatus& status) {  
        ClusterServer cluster_server;
        cluster_server.address = status.address;
        cluster_server.ping_port = status.ping_port;
        cluster_server.conn_port = status.connection_port;
        cluster_server.distance = status.distance;
        cluster_server.status = status.status;
        cluster_server.server_id = status.galaxy_id;
        cluster_server.not_recommended_flag = 0;
        cluster_server.max_chars = status.max_characters;
        cluster_server.max_pop = status.max_population;
        cluster_server.server_pop = status.server_population;
        
        message.servers.push_back(cluster_server);
    });
	
	return message;
}