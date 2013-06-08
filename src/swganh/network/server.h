// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <array>
#include <memory>

#include <boost/asio.hpp>

#include "swganh/network/server_interface.h"

namespace swganh {

class ByteBuffer;

namespace network {

// FORWARD DECLARATION
class Session;

/**
 * @brief An SOE Protocol Service.
 *
 * 
 */
class Server : public ServerInterface {
public:
    explicit Server(boost::asio::io_service& io_service);

    virtual ~Server();

    /**
     * @brief Starts the SOE Frontend Service.
     * 
     * @parama port The port to listen for messages on.
     */
    void StartListening(uint16_t port);
    
    /**
     * @brief
     */
    void StopListening(void);
    
    /**
     * @brief Sends a message on the wire to the target endpoint.
     */
    void SendTo(const boost::asio::ip::udp::endpoint& endpoint, swganh::ByteBuffer buffer);

    uint32_t max_receive_size();
    const boost::asio::ip::udp::endpoint& listen_endpoint() const { return listen_endpoint_; }
private:
    Server();
    
    void AsyncReceive();
    
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint current_remote_endpoint_;
    boost::asio::ip::udp::endpoint listen_endpoint_;
    std::array<uint8_t, 496> recv_buffer_;
    
    uint64_t bytes_recv_;
    uint64_t bytes_sent_;
    uint32_t max_receive_size_;
};

}} // namespace swganh::network
