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

#ifndef LIBANH_HASH_STRING_H_
#define LIBANH_HASH_STRING_H_

#include <cstdint>
#include <string>

namespace anh {

/*! \brief This class provides a utility for generating identifiers that are
 * easy to read and can be used as key values in the standard 
 * associative containers.
 */
class HashString {
public:
    /// Default constructor.
    HashString();

    /// Takes a regular std::string and stores a hash of it.
    HashString(const std::string& std_string); 

    /// Takes a human readable string and stores a hash of it.
    HashString(const char* ident_string);

    /// Takes an already hashed value and stores it.
    HashString(uint32_t ident);
    
    /// Conversion operator allows a hash string to be cast to a uint32_t
    operator uint32_t () const;

    /// Returns a 32bit hash representation of the string.
    uint32_t ident() const;

    /*! Uses a less-than comparison on two HashStrings.
     *
     * This is a requirement for this data type to be used as a key value in
     * containers like std::map.
     *
     * \param other The HashString to compare to the current one.
     * \return True if the ident is less than that of the other's, false if not.
     */
    bool operator<(const HashString& other) const;

    /*! Uses a greater-than comparison on two HashStrings.
     *
     * This is a requirement for this data type to be used as a key value in
     * containers like std::map.
     *
     * \param other The HashString to compare to the current one.
     * \return True if the ident is greater than that of the other's, false if not.
     */
    bool operator>(const HashString& other) const;

    /*! Compares two HashStrings to determine if they are equal.
     *
     * \param other The HashString to compare to the current one.
     * \return True if the two HashStrings are equal, false if not.
     */
    bool operator==(const HashString& other) const;

    /*! Compares two HashStrings to determine if they are not equal.
     *
     * \param other The HashString to compare to the current one.
     * \return True if the two HashStrings are not equal, false if they are.
     */
    bool operator!=(const HashString& other) const;

private:

    // \note: ident_ is stored as a void * not an int, so that in
    // the debugger it will show up as hex-values instead of
    // integer values. This is a bit more representative of what
    // we're doing here and makes it easy to allow external code
    // to assign event types as desired.
    void * ident_;              ///< A 32bit hash of the ident_string.
};

}  // namespace anh

namespace std {    
    // specialization of std::hash to make using
    // with unordered_maps easier by default.
    template <> struct hash<anh::HashString>
    {
        size_t operator()(const anh::HashString & x) const
        {
            return x.ident();
        }
    };
}  // namespace std

#endif  // LIBANH_HASH_STRING_H_