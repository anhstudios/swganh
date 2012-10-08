// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "hash_string.h"
#include "crc.h"

using namespace swganh;

HashString::HashString()
    : ident_(0)
{}

HashString::HashString(std::string ident_string)
    : ident_(reinterpret_cast<void*>(memcrc(ident_string)))
    , ident_string_(ident_string)
{}

HashString::HashString(const char* ident_string)
    : ident_(reinterpret_cast<void*>(memcrc(std::string(ident_string))))
    , ident_string_(ident_string)
{}

HashString::HashString(uint32_t ident)
    : ident_(reinterpret_cast<void*>(ident))
{}

HashString::operator uint32_t () const {
    return ident();
}

HashString::operator std::string () const {
    return ident_string();
}

uint32_t HashString::ident() const {
    return static_cast<uint32_t>(reinterpret_cast<uint64_t>(ident_));
}

const std::string& HashString::ident_string() const {
    return ident_string_;
}

bool HashString::operator<(const HashString& other) const {
    bool r = (ident() < other.ident());
    return r;
}

bool HashString::operator>(const HashString& other) const {
    bool r = (ident() > other.ident());
    return r;
}

bool HashString::operator==(const HashString& other) const {
    bool r = (ident() == other.ident());
    return r;
}

bool HashString::operator!=(const HashString& other) const {
    bool r = (ident() != other.ident());
    return r;
}
