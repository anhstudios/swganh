
#include "resolver.h"

#include <boost/asio.hpp>

using boost::asio::ip::udp;
using boost::asio::io_service;

std::string swganh::network::resolve_to_string(const std::string& hostname)
{
    io_service io;
    udp::resolver resolver(io);
    udp::resolver::query query(udp::v4(), hostname, "");
    udp::endpoint resolved_endpoint = *resolver.resolve(query);

    return resolved_endpoint.address().to_string();
}
