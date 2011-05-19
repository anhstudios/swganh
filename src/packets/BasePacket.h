#ifndef ANH_PACKETS_BASE_PACKET_H
#define ANH_PACKETS_BASE_PACKET_H

#include <anh/event_dispatcher/basic_event.h>
#include <anh/memory.h>
#include <packets/opcodes.h>
#include "session.h"
#include <cstdint>

// Originates on Client
namespace packets {
enum RoutingDestination
{
    CLIENT         = 0,
    SHARED_SERVICE = 1,
    ZONE           = 2,
    NONE           = 3
};
struct BasePacket
{
    BasePacket(){}
    explicit BasePacket(std::shared_ptr<network::Session> session_, RoutingDestination destination_)
    : destination(destination_)
    , session(session_)
	{}
    RoutingDestination destination;
    std::shared_ptr<network::Session> session;
};
typedef anh::event_dispatcher::BasicEvent<BasePacket> BasePacketEvent;
} // packets

#endif // ANH_PACKETS_BASE_PACKET_H