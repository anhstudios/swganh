// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <boost/test/unit_test.hpp>

#include "swganh/crc.h"

using swganh::memcrc;
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE ANH CRC
BOOST_AUTO_TEST_SUITE(ANHCRC)
/// This test shows how to find the 32bit checksum of a c-style string.
BOOST_AUTO_TEST_CASE(CanCrcCstyleStrings) {
    BOOST_CHECK_EQUAL(uint32_t(0x338BCFAC), memcrc("test"));
    BOOST_CHECK_EQUAL(uint32_t(0x2643D57C), memcrc("anothertest"));
    BOOST_CHECK_EQUAL(uint32_t(0x19522193), memcrc("aThirdTest"));
}

/// This test shows how to find the 32bit checksum of a std::string.
BOOST_AUTO_TEST_CASE(CanCrcStdStrings) {
    BOOST_CHECK_EQUAL(uint32_t(0x338BCFAC), memcrc(std::string("test")));
    BOOST_CHECK_EQUAL(uint32_t(0x2643D57C), memcrc(std::string("anothertest")));
    BOOST_CHECK_EQUAL(uint32_t(0x19522193), memcrc(std::string("aThirdTest")));
}
BOOST_AUTO_TEST_SUITE_END()