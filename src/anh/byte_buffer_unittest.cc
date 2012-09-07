// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <boost/test/unit_test.hpp>
#include "swganh/byte_buffer.h"

using namespace swganh;

namespace {

BOOST_AUTO_TEST_SUITE(ANHByteBuffer)

BOOST_AUTO_TEST_CASE(ByteBufferIsEmptyWhenCreated)
{
    ByteBuffer buffer;
    BOOST_CHECK_EQUAL(uint32_t(0), buffer.size());
}

BOOST_AUTO_TEST_CASE(ByteBufferDefaultCapacityIsZero)
{
    ByteBuffer buffer;
    BOOST_CHECK_EQUAL(uint32_t(0), buffer.capacity());
}

BOOST_AUTO_TEST_CASE(WritingIntReportsCorrectSizeAndCapacity)
{
    ByteBuffer buffer;
    buffer.write<int>(10);

    BOOST_CHECK_EQUAL(uint32_t(4), buffer.size());
    BOOST_CHECK_EQUAL(uint32_t(4), buffer.capacity());
}

BOOST_AUTO_TEST_CASE(WritingTwoIntsReportsCorrectSizeAndCapacity)
{
    ByteBuffer buffer;

    buffer.write<int>(10);
    BOOST_CHECK_EQUAL(uint32_t(4), buffer.size());
    BOOST_CHECK_EQUAL(uint32_t(4), buffer.capacity());

    buffer.write<int>(20);
    BOOST_CHECK_EQUAL(uint32_t(8), buffer.size());
    BOOST_CHECK_EQUAL(uint32_t(8), buffer.capacity());
}

BOOST_AUTO_TEST_CASE(CanReadIntWrittenToTheBuffer)
{
    ByteBuffer buffer;

    buffer.write<int>(10);
    BOOST_CHECK_EQUAL(10, buffer.read<int>());
}

BOOST_AUTO_TEST_CASE(CanReadTwoIntsWrittenToTheBuffer)
{
    ByteBuffer buffer;

    buffer.write<int>(10);
    buffer.write<int>(20);

    BOOST_CHECK_EQUAL(10, buffer.read<int>());
    BOOST_CHECK_EQUAL(20, buffer.read<int>());
}

BOOST_AUTO_TEST_CASE(ReadingPastBufferEndThrowsException)
{
    ByteBuffer buffer;
    BOOST_CHECK_EQUAL(uint32_t(0), buffer.size());

    BOOST_CHECK_THROW(buffer.read<int>(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(WritingStringReportsCorrectSizeAndCapacity)
{
    ByteBuffer buffer;
    BOOST_CHECK_EQUAL(uint32_t(0), buffer.size());
    BOOST_CHECK_EQUAL(uint32_t(0), buffer.capacity());

    buffer.write<std::string>(std::string("test string"));

    BOOST_CHECK_EQUAL(uint32_t(13), buffer.size());
    BOOST_CHECK_EQUAL(uint32_t(13), buffer.capacity());
}

BOOST_AUTO_TEST_CASE(CanReadStringWrittenToTheBuffer)
{
    ByteBuffer buffer;
    std::string test_string("test string data");

    buffer.write<std::string>(test_string);

    BOOST_CHECK_EQUAL(test_string, buffer.read<std::string>());
}

BOOST_AUTO_TEST_CASE(CanReadTwoStringsWrittenToTheBuffer)
{
    ByteBuffer buffer;
    std::string test_string1("first test string");
    std::string test_string2("second test string");

    buffer.write<std::string>(test_string1);
    buffer.write<std::string>(test_string2);

    BOOST_CHECK_EQUAL(test_string1, buffer.read<std::string>());
    BOOST_CHECK_EQUAL(test_string2, buffer.read<std::string>());
}

BOOST_AUTO_TEST_CASE(WritingUnicodeStringReportsCorrectSizeAndCapacity)
{
    ByteBuffer buffer;
    BOOST_CHECK_EQUAL(uint32_t(0), buffer.size());
    BOOST_CHECK_EQUAL(uint32_t(0), buffer.capacity());

    buffer.write<std::wstring>(std::wstring(L"test string"));

    // Length and Capacity should be size of int + size of string * size of wchar_t.
    BOOST_CHECK_EQUAL(sizeof(uint32_t) + (11 * 2), buffer.size());
    BOOST_CHECK_EQUAL(sizeof(uint32_t) + (11 * 2), buffer.capacity());
}

BOOST_AUTO_TEST_CASE(CanReadUnicodeStringWrittenToTheBuffer)
{
    ByteBuffer buffer;
    std::wstring test_string(L"test string data");

    buffer.write<std::wstring>(test_string);
    BOOST_CHECK_EQUAL(test_string.c_str(), buffer.read<std::wstring>().c_str());
}

BOOST_AUTO_TEST_CASE(UnicodeStringStoredAs16Bit)
{
    ByteBuffer buffer;
    std::wstring test_string(L"testdata");

    BOOST_CHECK_EQUAL(uint32_t(8), test_string.length());
    
    buffer.write<std::wstring>(test_string);

    BOOST_CHECK_EQUAL(uint32_t(8), buffer.read<uint32_t>());

    BOOST_CHECK_EQUAL('t', buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL(0, buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL('e', buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL(0, buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL('s', buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL(0, buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL('t', buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL(0, buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL('d', buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL(0, buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL('a', buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL(0, buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL('t', buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL(0, buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL('a', buffer.read<uint8_t>());
    BOOST_CHECK_EQUAL(0, buffer.read<uint8_t>());
}

BOOST_AUTO_TEST_CASE(CanClearBufferData)
{
    ByteBuffer buffer;
    BOOST_CHECK_EQUAL(uint32_t(0), buffer.size());

    buffer.write<int>(3);
    BOOST_CHECK_EQUAL(uint32_t(4), buffer.size());

    buffer.clear();
    BOOST_CHECK_EQUAL(uint32_t(0), buffer.size());
    BOOST_CHECK_EQUAL(uint32_t(0), buffer.read_position());
    BOOST_CHECK_EQUAL(uint32_t(0), buffer.write_position());
}

BOOST_AUTO_TEST_CASE(CanStreamData)
{
    ByteBuffer buffer;
    int testInt = 3;

    buffer << testInt;
    BOOST_CHECK_EQUAL(3, buffer.read<int>());
}

BOOST_AUTO_TEST_CASE(PeekingDataDoesNotMoveReadPosition)
{
    ByteBuffer buffer;
    buffer.write<int>(3);
    buffer.write<int>(10);

    // Peek the data twice, this should return the same integer value (3) both times.
    BOOST_CHECK_EQUAL(3, buffer.peek<int>());
    BOOST_CHECK_EQUAL(3, buffer.peek<int>());
}

BOOST_AUTO_TEST_CASE(CanReadAndWriteUnsignedData)
{
    ByteBuffer buffer;
    buffer.write<uint16_t>(3);

    BOOST_CHECK_EQUAL(uint32_t(2), buffer.size()); // First check that the right datasize was written.
    BOOST_CHECK_EQUAL(3, buffer.read<uint16_t>()); // Check that the value is correct.
}

BOOST_AUTO_TEST_CASE(CanPeekAtOffset)
{
    ByteBuffer buffer;
    buffer.write<int>(3);
    buffer.write<int>(32);
    buffer.write<int>(979);
    buffer.write<int>(5467);

    BOOST_CHECK_EQUAL(979, buffer.peekAt<int>(8));
    BOOST_CHECK_EQUAL(32, buffer.peekAt<int>(4));
}

BOOST_AUTO_TEST_CASE(CanWriteAtOffset)
{
    ByteBuffer buffer;
    buffer.write<int>(3);
    buffer.write<int>(32);
    buffer.write<int>(979);
    buffer.write<int>(5467);

    buffer.writeAt<int>(8, 52);
    buffer.writeAt<int>(4, 3532);

    BOOST_CHECK_EQUAL(52, buffer.peekAt<int>(8));
    BOOST_CHECK_EQUAL(3532, buffer.peekAt<int>(4));
}

BOOST_AUTO_TEST_CASE(CanAppendBuffers)
{
    ByteBuffer buffer1;
    buffer1.write<int>(0);
    buffer1.write<int>(1);
    buffer1.write<int>(2);

    ByteBuffer buffer2;
    buffer2.write<int>(3);
    buffer2.write<int>(4);
    buffer2.write<int>(5);

    buffer1.append(buffer2);

    BOOST_CHECK_EQUAL(6 * sizeof(int), buffer1.size());
    BOOST_CHECK_EQUAL(5, buffer1.peekAt<int>(5 * sizeof(int)));
}

BOOST_AUTO_TEST_CASE(CanSwapEndian)
{
    ByteBuffer buffer;
    
    buffer.write<char>(0);
    buffer.write<char>(0);
    buffer.write<char>(0);
    buffer.write<char>(2);
    
    BOOST_CHECK_EQUAL(uint32_t(2), buffer.peek<uint32_t>(true));

    // Start a new check with a 64bit value
    buffer.clear();
    buffer.write<char>(0);
    buffer.write<char>(0);
    buffer.write<char>(0);
    buffer.write<char>(0);
    buffer.write<char>(0);
    buffer.write<char>(0);
    buffer.write<char>(0);
    buffer.write<char>(2);

    BOOST_CHECK_EQUAL(uint64_t(2), buffer.peek<uint64_t>(true));
}

BOOST_AUTO_TEST_CASE(CanCompareByteBuffers) {    
    ByteBuffer buffer1;
    buffer1.write<int>(0);
    buffer1.write<int>(1);
    buffer1.write<int>(2);

    ByteBuffer buffer2;
    buffer2.write<int>(3);
    buffer2.write<int>(4);
    buffer2.write<int>(5);

    BOOST_CHECK(buffer1 == buffer1);
    BOOST_CHECK(buffer1 != buffer2);
}
BOOST_AUTO_TEST_CASE(CanWriteVectors)
{
    ByteBuffer buffer;
    std::vector<uint32_t> int_vects;
    int_vects.push_back(12345);
    int_vects.push_back(67890);
    buffer.write(int_vects);
    BOOST_CHECK_EQUAL(2 * sizeof(int), buffer.size());
}
}
BOOST_AUTO_TEST_SUITE_END()