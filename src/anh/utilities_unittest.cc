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

#include <boost/test/unit_test.hpp>

#include "anh/utilities.h"

BOOST_AUTO_TEST_SUITE(ANHUtility)

BOOST_AUTO_TEST_CASE(CanSwapEndianFor16BitSignedIntegral)
{
    int16_t big_endian = 0x0200;
    int16_t little_endian = 0x0002;

    int16_t test = anh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = anh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}
BOOST_AUTO_TEST_CASE(CanSwapEndianFor16BitUnsignedIntegral) {
    uint16_t big_endian = 0x0200;
    uint16_t little_endian = 0x0002;

    uint16_t test = anh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = anh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}

BOOST_AUTO_TEST_CASE(CanSwapEndianFor32BitSignedIntegral) {
    int32_t big_endian = 0x02000000;
    int32_t little_endian = 0x00000002;

    int32_t test = anh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = anh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}

BOOST_AUTO_TEST_CASE(CanSwapEndianFor32BitUnsignedIntegral) {
    uint32_t big_endian = 0x02000000;
    uint32_t little_endian = 0x00000002;

    uint32_t test = anh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = anh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}

BOOST_AUTO_TEST_CASE(CanSwapEndianFor64BitSignedIntegral) {    
    int64_t big_endian = 0x0200000000000000;
    int64_t little_endian = 0x0000000000000002;

    int64_t test = anh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = anh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}

BOOST_AUTO_TEST_CASE(CanSwapEndianFor64BitUnsignedIntegral) {
    uint64_t big_endian = 0x0200000000000000;
    uint64_t little_endian = 0x0000000000000002;

    uint64_t test = anh::swapEndian(big_endian);
    BOOST_CHECK_EQUAL(little_endian, test);

    test = anh::swapEndian(little_endian);
    BOOST_CHECK_EQUAL(big_endian, test);
}

BOOST_AUTO_TEST_SUITE_END()