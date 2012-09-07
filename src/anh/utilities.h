// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>

namespace swganh {

    namespace detail {
        //// @TODO Remove comment when visual studio begins supporting constexpr.
        /*constexpr*/ bool inline isBigEndian() {
            uint16_t x = 1;
            return !(*reinterpret_cast<char*>(&x));
        }
        
        template<typename T>
        T swapEndian_(T value, std::integral_constant<size_t, 1>) {
            return value;
        }

        template<typename T>
        T swapEndian_(T value, std::integral_constant<size_t, 2>) {
          typename std::make_unsigned<T>::type& tmp = reinterpret_cast<typename std::make_unsigned<T>::type&>(value);
            tmp = (tmp >> 8) | (tmp << 8);

            return value;
        }
        
        template<typename T>
        T swapEndian_(T value, std::integral_constant<size_t, 4>) {
            typename std::make_unsigned<T>::type& tmp = reinterpret_cast<typename std::make_unsigned<T>::type&>(value);
            tmp = (tmp >> 24) |
                   ((tmp & 0x00FF0000) >> 8) | ((tmp & 0x0000FF00) << 8) |
                   (tmp << 24);

            return value;
        }
        
        template<typename T>
        T swapEndian_(T value, std::integral_constant<size_t, 8>) {
            typename std::make_unsigned<T>::type& tmp = reinterpret_cast<typename std::make_unsigned<T>::type&>(value);
            tmp = (tmp >> 56) |
                ((tmp & 0x00FF000000000000ULL) >> 40) |
                ((tmp & 0x0000FF0000000000ULL) >> 24) |
                ((tmp & 0x000000FF00000000ULL) >> 8)  |
                ((tmp & 0x00000000FF000000ULL) << 8)  |
                ((tmp & 0x0000000000FF0000ULL) << 24) |
                ((tmp & 0x000000000000FF00ULL) << 40) |
                (tmp  << 56);

            return value;
        }
    }

    /*! Swaps the endianness of integral values and returns the results.
    *
    * @param value An integral value for which to swap the endianness.
    * @return A copy of the input parameter with its endianness swapped.
    */
    template<typename T>
    T swapEndian(T value) {
        static_assert(std::is_integral<int>::value, "swap_endian<T> requires T to be an integral type.");
        return detail::swapEndian_<T>(value, std::integral_constant<size_t, sizeof(T)>());
    }

    /*! Converts an integral value from host-byte order to little endian.
    *
    * @param value An integral value to convert to little endian.
    * @return The value converted to little endian order.
    */
    template<typename T>
    T hostToLittle(T value) {
        static_assert(std::is_integral<int>::value, "host_to_little<T> requires T to be an integral type.");
        return detail::isBigEndian() ? swapEndian(value) : value;
    }
    
    /*! Converts an integral value from host-byte order to big endian.
    *
    * @param value The value to convert to big endian
    * @return The value converted to big endian order.
    */
    template<typename T>
    T hostToBig(T value) {
        static_assert(std::is_integral<int>::value, "host_to_big<T> requires T to be an integral type.");
        return detail::isBigEndian() ? value : swapEndian(value);
    }
    
    /*! Converts an integral value from big endian to host-byte order.
    *
    * @param value The value to convert to host-byte order.
    * @return The value converted to host-byte order.
    */
    template<typename T>
    T bigToHost(T value) {
        static_assert(std::is_integral<int>::value, "big_to_host<T> requires T to be an integral type.");
        return detail::isBigEndian() ? value : swapEndian(value);
    }
    
    /*! Converts an integral value from little endian to host-byte order.
    *
    * @param value The value to convert to host-byte order.
    * @return The value converted to host-byte order.
    */
    template<typename T>
    T littleToHost(T value) {
        static_assert(std::is_integral<int>::value, "little_to_host<T> requires T to be an integral type.");
        return detail::isBigEndian() ? swapEndian(value) : value;
    }
        
    int KeyboardHit();

    char GetHitKey();

}  // namespace swganh
