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

#ifndef ANH_NETWORK_SOE_SESSION_H_
#define ANH_NETWORK_SOE_SESSION_H_

#include <array>
#include <functional>
#include <list>
#include <memory>
#include <vector>

#include <tbb/atomic.h>
#include <tbb/concurrent_queue.h>

#include <boost/asio.hpp>

#include "anh/network/soe/packet.h"
#include "anh/network/soe/protocol_packets.h"
#include "anh/network/soe/server_interface.h"

#ifdef SendMessage
#undef SendMessage
#endif

namespace anh {

// FORWARD DECLARATIONS
class ByteBuffer;

namespace network {
namespace soe {

typedef std::function<void (uint32_t, std::shared_ptr<ByteBuffer>)> DatachannelHandler;

/**
 * @brief An estabilished connection between a SOE Client and a SOE Service.
 */
class Session : public std::enable_shared_from_this<Session> {
public:
    /**
     * Adds itself to the Session Manager.
     */
    Session(boost::asio::ip::udp::endpoint remote_endpoint, ServerInterface* server);
    ~Session(void);

    /**
    * @return The current send sequence for the server.
    */
    uint16_t server_sequence() const;

    uint32_t receive_buffer_size() const;

    /**
     * Set the receive buffer size.
     */
    void receive_buffer_size(uint32_t receive_buffer_size);

    uint32_t crc_length() const;

    /**
     * Set the crc length for footers
     */
    void crc_length(uint32_t crc_length);
    
    /**
     * Sets the crc seed used to encrypt this session's messages.
     */
    void crc_seed(uint32_t crc_seed);

    /**
     * @return The crc seed used to encrypt this session's messages.
     */
    uint32_t crc_seed() const;

    void datachannel_handler(DatachannelHandler handler);

    /**
     * Get a list of all outgoing data channel messages that have not yet been acknowledged
     * by the remote end.
     *
     * @return List of unacknowledged data channel messages.
     */
    std::vector<std::shared_ptr<anh::ByteBuffer>> GetUnacknowledgedMessages() const;    

    /**
    * Sends a data channel message to the remote client.
    *
    * Increases the server sequence count by 1 for each individual packet sent to the 
    * remote end. This call can result in multiple packets being generated depending on 
    * the size of the payload and whether or not it needs to be fragmented.
    *
    * @param message The payload to send in the data channel message(s).
    */
    void SendMessage(anh::ByteBuffer message);
    
    /**
    * Sends a data channel message to the remote client.
    *
    * Increases the server sequence count by 1 for each individual packet sent to the 
    * remote end. This call can result in multiple packets being generated depending on 
    * the size of the payload and whether or not it needs to be fragmented.
    *
    * @param message The payload to send in the data channel message(s).
    */
    template<typename T>
    void SendMessage(const T& message) {
        auto message_buffer = server_->AllocateBuffer();
        message.serialize(*message_buffer);
        
        outgoing_data_messages_.push(message_buffer);
    }

    void HandleMessage(anh::ByteBuffer& message);

    /**
     * Clears each message pump.
     */
    void Update();

    /**
     * Closes the Session.
     */
    void Close(void);

    bool connected() { return connected_; }

    uint32_t connection_id() { return connection_id_; }

    boost::asio::ip::udp::endpoint& remote_endpoint() { return remote_endpoint_; }

    ServerInterface* server() { return server_; }

private:
    typedef std::list<std::pair<uint16_t, std::shared_ptr<anh::ByteBuffer>>> SequencedMessageMap;
    //typedef	std::map<uint16_t, std::shared_ptr<anh::ByteBuffer>>				SequencedMessageMap;
    
    typedef std::function<anh::ByteBuffer(uint16_t)> HeaderBuilder;

    void SendSequencedMessage_(HeaderBuilder header_builder, ByteBuffer message);    

    void handleSessionRequest_(SessionRequest& packet);
    void handleMultiPacket_(MultiPacket& packet);
    void handleDisconnect_(Disconnect& packet);
    void handlePing_(Ping& packet);
    void handleNetStatsClient_(NetStatsClient& packet);
    void handleChildDataA_(ChildDataA& packet);
    void handleDataFragA_(DataFragA& packet);
    void handleAckA_(AckA& packet);
    void handleOutOfOrderA_(OutOfOrderA& packet);

    void SendSoePacket_(std::shared_ptr<anh::ByteBuffer> message);

    bool SequenceIsValid_(const uint16_t& sequence);
    void AcknowledgeSequence_(const uint16_t& sequence);

    boost::asio::ip::udp::endpoint		remote_endpoint_; // ip_address
    ServerInterface*					server_; // owner


    SequencedMessageMap					sent_messages_;

    bool								connected_;

    std::shared_ptr<DatachannelHandler> datachannel_handler_;

    // SOE Session Variables
    uint32_t							connection_id_;
    uint32_t							receive_buffer_size_;
    uint32_t							crc_length_;
    uint32_t                            crc_seed_;

    // Sequences
    uint16_t							last_acknowledged_sequence_;
    uint16_t							next_client_sequence_;
    uint16_t							current_client_sequence_;
    tbb::atomic<uint16_t>				server_sequence_;

    uint32_t							next_frag_size_;

    // Net Stats
    NetStatsServer						server_net_stats_;

    ChildDataA							outgoing_data_message_;
    
    tbb::concurrent_queue<std::shared_ptr<anh::ByteBuffer>> outgoing_data_messages_;

    std::list<anh::ByteBuffer>			incoming_fragmented_messages_;
    uint16_t							incoming_fragmented_total_len_;
    uint16_t							incoming_fragmented_curr_len_;
};

}}} // namespace anh::network::soe

#endif  // ANH_NETWORK_SOE_SESSION_H_
