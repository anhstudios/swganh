// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <boost/test/unit_test.hpp>

#include "swganh/utilities.h"

BOOST_AUTO_TEST_SUITE(ANHUtility)

BOOST_AUTO_TEST_CASE(CanSwapEndianFor16BitSignedIntegral)
{
    int16_t big_endian = 0x0200;
    int16_t little_endian = 0x0002;

    int16_t test = swganh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = swganh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}
BOOST_AUTO_TEST_CASE(CanSwapEndianFor16BitUnsignedIntegral) {
    uint16_t big_endian = 0x0200;
    uint16_t little_endian = 0x0002;

    uint16_t test = swganh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = swganh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}

BOOST_AUTO_TEST_CASE(CanSwapEndianFor32BitSignedIntegral) {
    int32_t big_endian = 0x02000000;
    int32_t little_endian = 0x00000002;

    int32_t test = swganh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = swganh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}

BOOST_AUTO_TEST_CASE(CanSwapEndianFor32BitUnsignedIntegral) {
    uint32_t big_endian = 0x02000000;
    uint32_t little_endian = 0x00000002;

    uint32_t test = swganh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = swganh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}

BOOST_AUTO_TEST_CASE(CanSwapEndianFor64BitSignedIntegral) {    
    int64_t big_endian = 0x0200000000000000;
    int64_t little_endian = 0x0000000000000002;

    int64_t test = swganh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = swganh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}

BOOST_AUTO_TEST_CASE(CanSwapEndianFor64BitUnsignedIntegral) {
    uint64_t big_endian = 0x0200000000000000;
    uint64_t little_endian = 0x0000000000000002;

    uint64_t test = swganh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = swganh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}

BOOST_AUTO_TEST_SUITE_END()