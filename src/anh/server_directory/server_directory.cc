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

#include <anh/server_directory/server_directory.h>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/server_directory/datastore.h>
#include <anh/server_directory/server_directory_events.h>

using namespace anh::server_directory;
using namespace anh::event_dispatcher;
using namespace std;

ServerDirectory::ServerDirectory(shared_ptr<DatastoreInterface> datastore,
        shared_ptr<EventDispatcherInterface> event_dispatcher) 
    : datastore_(datastore)
    , event_dispatcher_(event_dispatcher)
{}

ServerDirectory::ServerDirectory(
    shared_ptr<DatastoreInterface> datastore, 
    std::shared_ptr<EventDispatcherInterface> event_dispatcher,
    const string& cluster_name, 
    const string& version , 
    bool create_cluster) 
    : datastore_(datastore)
    , event_dispatcher_(event_dispatcher)
    , active_cluster_(nullptr)
    , active_process_(nullptr)
{
    active_cluster_ = datastore->findClusterByName(cluster_name);
    
    if (!active_cluster_) {
        // if no cluster was found and no request to create it was made, fail now
        if (! create_cluster) {
            throw InvalidClusterError(std::string("Attempted to join an invalid cluster: ").append(cluster_name));
        }

        if (!(active_cluster_ = datastore->createCluster(cluster_name, version))) {
            throw InvalidClusterError(std::string("Attempt to create cluster failed: ").append(cluster_name));
        }
    }
}

shared_ptr<Cluster> ServerDirectory::cluster() const {
    return active_cluster_;
}

shared_ptr<Process> ServerDirectory::process() const {
    return active_process_;
}

bool ServerDirectory::registerProcess(const string& name, const string& process_type, const string& version, const string& address, uint16_t tcp_port, uint16_t udp_port, uint16_t ping_port) {
    if (active_process_ = datastore_->createProcess(active_cluster_, name, process_type, version, address, tcp_port, udp_port, ping_port)) {
        
        // trigger the event to let any listeners we have added the process
        auto event_ = make_shared_event("RegisterProcess", *active_process_);
        event_dispatcher_->trigger(event_);
        return true;
    }

    return false;
}

bool ServerDirectory::removeProcess(shared_ptr<Process>& process) {
    if (datastore_->deleteProcessById(process->id())) {
        if (active_process_ && process->id() == active_process_->id()) {
            // before we clear out the process
            active_process_ = nullptr;
        }
        // trigger the event to let any listeners we have removed the process
        auto remove_event = make_shared_event("RemoveProcess", *process);
        event_dispatcher_->trigger(remove_event);

        process = nullptr;

        return true;
    }

    return false;
}

void ServerDirectory::updateProcessStatus(shared_ptr<Process>& process, int32_t new_status) {
    process->status(new_status);
    datastore_->saveProcess(process);
}

bool ServerDirectory::makePrimaryProcess(shared_ptr<Process> process) {
    active_cluster_->primary_id(process->id());
    return true;
}

void ServerDirectory::pulse() {
    if (active_process_) {
        std::string last_pulse = "";
        if (active_cluster_ && active_cluster_->primary_id() != 0) {
            last_pulse = datastore_->getClusterTimestamp(active_cluster_);
        } else {
            last_pulse = boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time());
        }

        active_process_->last_pulse(last_pulse);
        datastore_->saveProcess(active_process_);
    }

    if (active_cluster_) {
        active_cluster_ = datastore_->findClusterById(active_cluster_->id());
    }
}

ClusterList ServerDirectory::getClusterSnapshot() const {
    return datastore_->getClusterList();
}

ProcessList ServerDirectory::getProcessSnapshot(shared_ptr<Cluster> cluster) const {
    return datastore_->getProcessList(cluster->id());
}

