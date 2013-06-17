// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <atomic>
#include <list>
#include <memory>
#include <tuple>
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
#include <boost/optional.hpp>

#include "swganh/network/protocol_packets.h"
#include "swganh/network/server_interface.h"

#include "swganh/network/filters.h"

namespace swganh {

// FORWARD DECLARATIONS
class ByteBuffer;

namespace network {

/**
 * @brief An estabilished connection between a SOE Client and a SOE Service.
 */
class Session : public std::enable_shared_from_this<Session> {
public:
    /**
     * Adds itself to the Session Manager.
     */
    Session(ServerInterface* server, boost::asio::io_service& cpu_pool, boost::asio::ip::udp::endpoint remote_endpoint);
    virtual ~Session();

	typedef std::function<void(uint16_t /* sequence */)> SequencedCallback;
	typedef std::list<SequencedCallback> SequencedCallbacks;
    typedef std::list<std::tuple<uint16_t, ByteBuffer, SequencedCallbacks>> SequencedMessagesList;

    uint32_t max_data_size() const { return receive_buffer_size_ - crc_length_ - 3; }

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
    * Sends a data channel message to the remote client.
    *
    * Increases the server sequence count by 1 for each individual packet sent to the
    * remote end. This call can result in multiple packets being generated depending on
    * the size of the payload and whether or not it needs to be fragmented.
    *
    * @param message The payload to send in the data channel message(s).
    */
    void SendTo(ByteBuffer message, boost::optional<SequencedCallback> callback = boost::optional<SequencedCallback>());

    /**
    * Sends a data channel message to the remote client.
    *
    * Increases the server sequence count by 1 for each individual packet sent to the
    * remote end. This call can result in multiple packets being generated depending on
    * the size of the payload and whether or not it needs to be fragmented.
    *
    * @param message The payload to send in the data channel message(s).
    */
    void SendTo(ByteBuffer message, SequencedCallbacks callbacks);

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
    void SendTo(const T& message, boost::optional<SequencedCallback> callback = boost::optional<SequencedCallback>()) {
        SendTo(Serialize(message), callback);
    }

    void HandleMessage(swganh::ByteBuffer message);

    void HandleProtocolMessage(swganh::ByteBuffer message);

    /**
     * Closes the Session.
     */
    void Close(void);

    bool connected() { return connected_; }

    uint32_t connection_id() { return connection_id_; }

    boost::asio::ip::udp::endpoint& remote_endpoint() { return remote_endpoint_; }

private:
    typedef std::list<std::pair<uint16_t, swganh::ByteBuffer>> SequencedMessageMap;

    typedef swganh::ByteBuffer(*HeaderBuilder)(uint16_t);

    virtual void OnClose() {}

    void handleSessionRequest_(SessionRequest packet);
    void handleMultiPacket_(MultiPacket packet);
    void handleDisconnect_(Disconnect packet);
    void handlePing_(Ping packet);
    void handleNetStatsClient_(NetStatsClient packet);
    void handleChildDataA_(ChildDataA packet);
    void handleDataFragA_(DataFragA packet);
    void handleAckA_(AckA packet);
    void handleOutOfOrderA_(OutOfOrderA packet);
    swganh::ByteBuffer SendSoePacket_(swganh::ByteBuffer message);
    void SendFragmentedPacket_(swganh::ByteBuffer message, SequencedCallbacks callbacks);    
    void SendSequencedMessage_(uint16_t header, ByteBuffer message, SequencedCallbacks callbacks = SequencedCallbacks());

    void HandleProtocolMessageInternal(swganh::ByteBuffer message);

    bool AcknowledgeSequence_(const uint16_t& sequence);

    boost::asio::ip::udp::endpoint		remote_endpoint_; // ip_address
    ServerInterface*					server_; // owner
    boost::asio::strand strand_;

    SequencedMessagesList unacknowledged_messages_;

    bool								connected_;

    // SOE Session Variables
    uint32_t							connection_id_;
    uint32_t							receive_buffer_size_;
    uint32_t							crc_length_;
    uint32_t                            crc_seed_;

    // Sequences
    uint16_t							next_client_sequence_;
    uint16_t							current_client_sequence_;
    std::atomic<uint16_t>				server_sequence_;

    // Net Stats
    NetStatsServer						server_net_stats_;

    uint16_t incoming_fragmented_total_len_;
    ByteBuffer incoming_frag_;

    CompressionFilter compression_filter_;
    CrcInFilter crc_input_filter_;
    CrcOutFilter crc_output_filter_;
    DecompressionFilter decompression_filter_;
    DecryptionFilter decryption_filter_;
    EncryptionFilter encryption_filter_;
    SecurityFilter security_filter_;
};

}} // namespace swganh::network
