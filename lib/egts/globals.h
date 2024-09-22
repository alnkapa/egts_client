// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <span>
#include <vector>

namespace egts::v1
{

using buffer_type = std::vector<unsigned char>;
using payload_type = std::span<const unsigned char>;

inline bool
has_remaining_bytes(payload_type buffer, payload_type::iterator ptr, std::size_t x)
{
    if (ptr < buffer.begin() || ptr > buffer.end())
    {
        return false;
    }
    return std::distance(ptr, buffer.end()) >= x;
};

} // namespace egts::v1

inline std::ostream &
operator<<(std::ostream &os, const egts::v1::buffer_type &buffer)
{
    os << "Buffer: [";
    for (size_t i = 0; i < buffer.size(); ++i)
    {
        os << std::hex << static_cast<int>(buffer[i]);
    }
    os << std::dec << "]";
    return os;
}

inline std::ostream &
operator<<(std::ostream &os, const egts::v1::payload_type &buffer)
{
    os << "Payload: [";
    for (size_t i = 0; i < buffer.size(); ++i)
    {
        os << std::hex << static_cast<int>(buffer[i]);
    }
    os << std::dec << "]";
    return os;
}

#endif // GLOBALS_H