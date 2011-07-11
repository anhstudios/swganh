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

#include "connection/connection_service.h"

#include <iostream>
#include <glog/logging.h>

#include "anh/crc.h"
#include "anh/event_dispatcher/basic_event.h"

#include "anh/network/soe/packet.h"
#include "anh/network/soe/session.h"

#include "swganh/base/swg_message_handler.h"
#include "swganh/character/character_data.h"

#include "swganh/scene/messages/cmd_scene_ready.h"
#include "swganh/scene/messages/cmd_start_scene.h"
#include "swganh/scene/messages/scene_create_object_by_crc.h"
#include "swganh/scene/messages/scene_end_baselines.h"

#include "connection/messages/client_permissions_message.h"
#include "connection/messages/select_character.h"
#include "connection/messages/client_create_character.h"
#include "connection/messages/client_create_character_success.h"
#include "connection/messages/client_create_character_failed.h"

using namespace anh;
using namespace event_dispatcher;
using namespace connection;
using namespace messages;
using namespace swganh;
using namespace character;
using namespace scene::messages;
using namespace std;

ConnectionService::ConnectionService(shared_ptr<EventDispatcherInterface> event_dispatcher) 
    : BaseService(event_dispatcher)
    , listen_port_(0) {
        
    soe_server_.reset(new network::soe::Server(swganh::base::SwgMessageHandler(event_dispatcher)));

    this->event_dispatcher(event_dispatcher);
}

ConnectionService::~ConnectionService() {}

void ConnectionService::DescribeConfigOptions(boost::program_options::options_description& description) {
    description.add_options()
        ("service.connection.udp_port", boost::program_options::value<uint16_t>(&listen_port_),
            "The port the connection service will listen for incoming client connections on")
    ;
}

void ConnectionService::onStart() {
    soe_server_->Start(listen_port_);
}

void ConnectionService::Update() {
    soe_server_->Update();
}

void ConnectionService::onStop() {
    soe_server_->Shutdown();
}

void ConnectionService::subscribe() {
    soe_server_->event_dispatcher(event_dispatcher_);
    
    event_dispatcher_->subscribe("CmdSceneReady", bind(&ConnectionService::HandleCmdSceneReady_, this, placeholders::_1));
    event_dispatcher_->subscribe("ClientIdMsg", bind(&ConnectionService::HandleClientIdMsg_, this, placeholders::_1));
    event_dispatcher_->subscribe("SelectCharacter", bind(&ConnectionService::HandleSelectCharacter_, this, placeholders::_1));
    event_dispatcher_->subscribe("ClientCreateCharacter", bind(&ConnectionService::HandleClientCreateCharacter_, this, placeholders::_1));
}

shared_ptr<CharacterServiceInterface> ConnectionService::character_service() {
    return character_service_;
}

void ConnectionService::character_service(shared_ptr<CharacterServiceInterface> character_service) {
    character_service_ = character_service;
}

bool ConnectionService::HandleCmdSceneReady_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event) {
    DLOG(WARNING) << "Handling CmdSceneReady";
    auto remote_event = static_pointer_cast<BasicEvent<anh::network::soe::Packet>>(incoming_event);
    
    ByteBuffer buffer;
    
    CmdSceneReady ready_scene;
    
    remote_event->session()->SendMessage(ready_scene);

    return true;
}

bool ConnectionService::HandleClientIdMsg_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event) {
    DLOG(WARNING) << "Handling ClientIdMsg";
    auto remote_event = static_pointer_cast<BasicEvent<anh::network::soe::Packet>>(incoming_event);
    
    ClientPermissionsMessage client_permissions;
    client_permissions.galaxy_available = 1;
    client_permissions.available_character_slots = 2;
    client_permissions.unlimited_characters = 0;

    remote_event->session()->SendMessage(client_permissions);

    return true;
}


bool ConnectionService::HandleSelectCharacter_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event) {
    DLOG(WARNING) << "Handling SelectCharacter";
    auto remote_event = static_pointer_cast<BasicEvent<anh::network::soe::Packet>>(incoming_event);
    SelectCharacter select_character;
    select_character.serialize(*remote_event->message());
    
    swganh::character::CharacterLoginData character = character_service()->GetLoginCharacter(select_character.character_id);
    CmdStartScene start_scene;
    start_scene.ignore_layout = 0;
    start_scene.character_id = character.character_id;
    start_scene.terrain_map = character.terrain_map;
    start_scene.position = character.position;
    start_scene.race_template = character.race_template;
    start_scene.galaxy_time = 0;
        
    remote_event->session()->SendMessage(start_scene);

    SceneCreateObjectByCrc scene_object;
    scene_object.object_id = character.character_id;
    scene_object.orientation = character.orientation;
    scene_object.position = character.position;
    scene_object.object_crc = anh::memcrc(character.race_template);
    scene_object.byte_flag = 0;
    
    remote_event->session()->SendMessage(scene_object);
    
    SceneEndBaselines scene_object_end;
    scene_object_end.object_id = character.character_id;
    
    remote_event->session()->SendMessage(scene_object_end);

    return true;
}
bool ConnectionService::HandleClientCreateCharacter_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event) {
    DLOG(WARNING) << "Handling ClientCreateCharacter";
    auto remote_event = static_pointer_cast<BasicEvent<anh::network::soe::Packet>>(incoming_event);
    ClientCreateCharacter create_character;
    create_character.deserialize(*remote_event->message());

    uint64_t character_id;
    string error_code;
    tie(character_id, error_code) = character_service()->CreateCharacter(create_character);
    if (error_code.length() > 0)
    {
        ClientCreateCharacterFailed failed;
        failed.stf_file = "ui";
        failed.error_string = error_code;
        remote_event->session()->SendMessage(failed);
    }
    else
    {
        ClientCreateCharacterSuccess success;
        success.character_id = character_id;
        remote_event->session()->SendMessage(success);
    }
    
    return true;
}
