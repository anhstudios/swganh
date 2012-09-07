// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>

namespace swganh {

/** 
 * @brief Calculates a 32-bit checksum of a c-style string.
 *
 * Variation on the memcrc function which is part of the cksum utility.
 *
 * @see http://www.opengroup.org/onlinepubs/009695399/utilities/cksum.html
 *
 * @param source_string The string to use as the basis for generating the checksum.
 * @param length The length of the source_string.
 * @return A 32-bit checksum of the string.
 */
uint32_t memcrc(const char* source_string, uint32_t length);

/** 
 * @brief Calculates a 32-bit checksum of a std::string.
 *
 * Variation on the memcrc function which is part of the cksum utility.
 *
 * @see http://www.opengroup.org/onlinepubs/009695399/utilities/cksum.html
 *
 * @param source_string The string to use as the basis for generating the checksum.
 * @return A 32-bit checksum of the string.
 */
uint32_t memcrc(const std::string& source_string);

/**
 * @brief Calculates a 32-bit checksum of a buffer using a custom seed.
 *
 * Variation on the memcrc function which is part of the cksum utility.
 *
 * @param source_string The string to use as the basis for generating the checksum.
 * @param length The length of the source_string.
 * @param seed Crc Seed.
 * @return A 32-bit checksum of the string.
 */
uint32_t memcrc(const unsigned char* src_buffer, uint32_t length, uint32_t seed);

}  // namespace swganh
