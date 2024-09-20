// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <span>
#include <vector>

namespace egts::v1
{

using frame_buffer_type = std::vector<unsigned char>;
using record_payload_type = std::span<const unsigned char>;

inline bool
has_remaining_bytes(record_payload_type buffer, record_payload_type::iterator ptr, std::size_t x)
{
    if (ptr < buffer.begin() || ptr > buffer.end())
    {
        return false;
    }
    return std::distance(ptr, buffer.end()) >= x;
};

} // namespace egts::v1

#endif // GLOBALS_H