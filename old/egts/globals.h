// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <iterator> // std::distance
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
operator<<(std::ostream &os, egts::v1::payload_type buffer)
{
    os << "[";
    std::ios_base::fmtflags f(os.flags());
    for (size_t i = 0; i < buffer.size(); ++i)
    {
        os << "0x" << std::hex << static_cast<int>(buffer[i]);
        if (i + 1 < buffer.size())
        {
            os << ",";
        }
    }
    os.flags(f);
    os << "]";
    return os;
}

inline egts::v1::buffer_type
operator+=(egts::v1::buffer_type &lhs, egts::v1::buffer_type &&rhs)
{
    lhs.insert(
        lhs.end(),
        std::make_move_iterator(rhs.begin()),
        std::make_move_iterator(rhs.end()));

    return lhs;
}

#endif // GLOBALS_H