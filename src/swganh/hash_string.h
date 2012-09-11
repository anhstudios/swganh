// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>

namespace swganh {

/*! \brief This class provides a utility for generating identifiers that are
 * easy to read and can be used as key values in the standard 
 * associative containers.
 */
class HashString {
public:
    /// Default constructor.
    HashString();

    /// Takes a regular std::string and stores a hash of it.
    HashString(std::string ident_string);

    /// Takes a regular std::string and stores a hash of it.
    HashString(const char* ident_string);

    /// Takes an pre-hashed value and stores it.
    HashString(uint32_t ident);
    
    /// Conversion operator allows a hash string to be cast to a uint32_t
    operator uint32_t () const;

    /// Conversion operator allows a hash string to be cast to a std::string
    operator std::string () const;

    /// Returns a 32bit hash representation of the string.
    uint32_t ident() const;

    /// Returns a human readable identifier. Is empty if created from a pre-hashed value.
    const std::string& ident_string() const;

    /*! Uses a less-than comparison on two HashStrings.
     *
     * This is a requirement for this data type to be used as a key value in
     * containers like std::map.
     *
     * @param other The HashString to compare to the current one.
     * @return True if the ident is less than that of the other's, false if not.
     */
    bool operator<(const HashString& other) const;

    /*! Uses a greater-than comparison on two HashStrings.
     *
     * This is a requirement for this data type to be used as a key value in
     * containers like std::map.
     *
     * @param other The HashString to compare to the current one.
     * @return True if the ident is greater than that of the other's, false if not.
     */
    bool operator>(const HashString& other) const;

    /*! Compares two HashStrings to determine if they are equal.
     *
     * @param other The HashString to compare to the current one.
     * @return True if the two HashStrings are equal, false if not.
     */
    bool operator==(const HashString& other) const;

    /*! Compares two HashStrings to determine if they are not equal.
     *
     * @param other The HashString to compare to the current one.
     * @return True if the two HashStrings are not equal, false if they are.
     */
    bool operator!=(const HashString& other) const;

private:

    // \note: ident_ is stored as a void * not an int, so that in
    // the debugger it will show up as hex-values instead of
    // integer values. This is a bit more representative of what
    // we're doing here and makes it easy to allow external code
    // to assign event types as desired.
    void * ident_;              ///< A 32bit hash of the ident_string.
    std::string ident_string_;
};

}  // namespace swganh

namespace std {    
    // specialization of std::hash to make using
    // with unordered_maps easier by default.
    template <> struct hash<swganh::HashString>
    {
        size_t operator()(const swganh::HashString & x) const
        {
            return x.ident();
        }
    };
}  // namespace std
