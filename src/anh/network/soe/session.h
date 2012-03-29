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

#include <atomic>
#include <list>
#include <memory>
#include <vector>

#ifdef WIN32
#include <concurrent_queue.h>
#else
#include <tbb/concurrent_queue.h>

namespace Concurrency {
    using ::tbb::concurrent_queue;
}

#endif

#include <boost/asio.hpp>

#include "anh/network/soe/protocol_packets.h"
#include "anh/network/soe/server_interface.h"

#include "anh/network/soe/filters/crc_in_filter.h"
#include "anh/network/soe/filters/decryption_filter.h"
#include "anh/network/soe/filters/decompression_filter.h"
#include "anh/network/soe/filters/compression_filter.h"
#include "anh/network/soe/filters/crc_out_filter.h"
#include "anh/network/soe/filters/encryption_filter.h"
#include "anh/network/soe/filters/security_filter.h"

namespace anh {

// FORWARD DECLARATIONS
class ByteBuffer;

namespace network {
namespace soe {

/**
 * @brief An estabilished connection between a SOE Client and a SOE Service.
 */
class Session : public std::enable_shared_from_this<Session> {
public:
    /**
     * Adds itself to the Session Manager.
     */
    Session(ServerInterface* server, boost::asio::io_service& io_service, boost::asio::ip::udp::endpoint remote_endpoint);
    ~Session();

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
    void SendTo(anh::ByteBuffer message);

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
    void SendTo(const T& message) {
        auto message_buffer = server_->AllocateBuffer();
        message.Serialize(*message_buffer);

        outgoing_data_messages_.push(message_buffer);
    }

    void HandleMessage(const std::shared_ptr<anh::ByteBuffer>& message);

    void HandleProtocolMessage(const std::shared_ptr<anh::ByteBuffer>& message);

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

    ServerInterface* server();

private:
    typedef std::list<std::pair<uint16_t, std::shared_ptr<anh::ByteBuffer>>> SequencedMessageMap;

    typedef anh::ByteBuffer(*HeaderBuilder)(uint16_t);

    void SendSequencedMessage_(HeaderBuilder header_builder, ByteBuffer message);

    virtual void OnClose() {}

    void handleSessionRequest_(SessionRequest& packet);
    void handleMultiPacket_(MultiPacket& packet);
    void handleDisconnect_(Disconnect& packet);
    void handlePing_(Ping& packet);
    void handleNetStatsClient_(NetStatsClient& packet);
    void handleChildDataA_(ChildDataA& packet);
    void handleDataFragA_(DataFragA& packet);
    void handleAckA_(AckA& packet);
    void handleOutOfOrderA_(OutOfOrderA& packet);
    void SendSoePacket_(const std::shared_ptr<anh::ByteBuffer>& message);

    bool SequenceIsValid_(const uint16_t& sequence);
    void AcknowledgeSequence_(const uint16_t& sequence);

    boost::asio::ip::udp::endpoint		remote_endpoint_; // ip_address
    ServerInterface*					server_; // owner
    boost::asio::strand strand_;

    SequencedMessageMap					sent_messages_;

    bool								connected_;

    // SOE Session Variables
    uint32_t							connection_id_;
    uint32_t							receive_buffer_size_;
    uint32_t							crc_length_;
    uint32_t                            crc_seed_;

    // Sequences
    uint16_t							last_acknowledged_sequence_;
    uint16_t							next_client_sequence_;
    uint16_t							current_client_sequence_;
    std::atomic<uint16_t>				server_sequence_;

    uint32_t							next_frag_size_;

    // Net Stats
    NetStatsServer						server_net_stats_;

    Concurrency::concurrent_queue<std::shared_ptr<anh::ByteBuffer>> outgoing_data_messages_;

    std::list<anh::ByteBuffer>			incoming_fragmented_messages_;
    uint16_t							incoming_fragmented_total_len_;
    uint16_t							incoming_fragmented_curr_len_;

    filters::CompressionFilter compression_filter_;
    filters::CrcInFilter crc_input_filter_;
    filters::CrcOutFilter crc_output_filter_;
    filters::DecompressionFilter decompression_filter_;
    filters::DecryptionFilter decryption_filter_;
    filters::EncryptionFilter encryption_filter_;
    filters::SecurityFilter security_filter_;
};

}}} // namespace anh::network::soe

#endif  // ANH_NETWORK_SOE_SESSION_H_
