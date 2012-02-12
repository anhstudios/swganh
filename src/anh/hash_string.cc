
#include "hash_string.h"
#include "crc.h"

using namespace anh;

HashString::HashString()
    : ident_(0)
{}

HashString::HashString(const std::string &std_string)
    : ident_(reinterpret_cast<void*>(memcrc(std_string)))
{}

HashString::HashString(const char* ident_string)
    : ident_(reinterpret_cast<void*>(memcrc(std::string(ident_string))))
{}

HashString::HashString(uint32_t ident)
    : ident_(reinterpret_cast<void*>(ident))
{}

HashString::operator uint32_t () const {
    return ident();
}

uint32_t HashString::ident() const {
    return static_cast<uint32_t>(reinterpret_cast<uint64_t>(ident_));
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
