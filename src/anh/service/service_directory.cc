/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <anh/service/service_directory.h>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/service/datastore.h>

using namespace anh::service;
using namespace anh::event_dispatcher;
using namespace std;

ServiceDirectory::ServiceDirectory(shared_ptr<DatastoreInterface> datastore,
        shared_ptr<EventDispatcherInterface> event_dispatcher) 
    : datastore_(datastore)
    , event_dispatcher_(event_dispatcher)
{}

ServiceDirectory::ServiceDirectory(
    shared_ptr<DatastoreInterface> datastore, 
    std::shared_ptr<EventDispatcherInterface> event_dispatcher,
    const string& galaxy_name, 
    const string& version , 
    bool create_galaxy) 
    : datastore_(datastore)
    , event_dispatcher_(event_dispatcher)
    , active_galaxy_(nullptr)
    , active_service_(nullptr)
{
    joinGalaxy(galaxy_name, version, create_galaxy);
}

shared_ptr<Galaxy> ServiceDirectory::galaxy() const {
    return active_galaxy_;
}

shared_ptr<ServiceDescription> ServiceDirectory::service() const {
    return active_service_;
}


void ServiceDirectory::joinGalaxy(const std::string& galaxy_name, const std::string& version, bool create_galaxy) {    
    active_galaxy_ = datastore_->findGalaxyByName(galaxy_name);
    
    if (!active_galaxy_) {
        // if no galaxy was found and no request to create it was made, fail now
        if (! create_galaxy) {
            throw InvalidGalaxyError(std::string("Attempted to join an invalid galaxy: ").append(galaxy_name));
        }

        if (!(active_galaxy_ = datastore_->createGalaxy(galaxy_name, version))) {
            throw InvalidGalaxyError(std::string("Attempt to create galaxy failed: ").append(galaxy_name));
        }
    }
}

bool ServiceDirectory::registerService(const string& name, const string& service_type, const string& version, const string& address, uint16_t tcp_port, uint16_t udp_port, uint16_t ping_port) {
    if (active_service_ = datastore_->createService(active_galaxy_, name, service_type, version, address, tcp_port, udp_port, ping_port)) {
        
        // trigger the event to let any listeners we have added the service
        auto event_ = make_shared_event("RegisterService", *active_service_);
        event_dispatcher_->trigger(event_);
        return true;
    }

    return false;
}

bool ServiceDirectory::removeService(shared_ptr<ServiceDescription>& service) {
    if (datastore_->deleteServiceById(service->id())) {
        if (active_service_ && service->id() == active_service_->id()) {
            // before we clear out the service
            active_service_ = nullptr;
        }
        // trigger the event to let any listeners we have removed the service
        auto remove_event = make_shared_event("RemoveService", *service);
        event_dispatcher_->trigger(remove_event);

        service = nullptr;

        return true;
    }

    return false;
}

void ServiceDirectory::updateServiceStatus(shared_ptr<ServiceDescription>& service, int32_t new_status) {
    service->status(new_status);
    datastore_->saveService(service);
}

bool ServiceDirectory::makePrimaryService(shared_ptr<ServiceDescription> service) {
    active_galaxy_->primary_id(service->id());
    return true;
}

void ServiceDirectory::pulse() {
    if (active_service_) {
        std::string last_pulse = "";
        if (active_galaxy_ && active_galaxy_->primary_id() != 0) {
            last_pulse = datastore_->getGalaxyTimestamp(active_galaxy_);
        } else {
            last_pulse = boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time());
        }

        active_service_->last_pulse(last_pulse);
        datastore_->saveService(active_service_);
    }

    if (active_galaxy_) {
        active_galaxy_ = datastore_->findGalaxyById(active_galaxy_->id());
    }
}

GalaxyList ServiceDirectory::getGalaxySnapshot() const {
    return datastore_->getGalaxyList();
}

ServiceList ServiceDirectory::getServiceSnapshot(shared_ptr<Galaxy> galaxy) const {
    return datastore_->getServiceList(galaxy->id());
}

