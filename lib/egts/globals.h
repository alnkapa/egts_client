// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <span>
#include <vector>

namespace egts::v1
{

using frame_buffer_type = std::vector<unsigned char>;
using record_payload_type = std::span<const unsigned char>;

} // namespace egts::v1

#endif // GLOBALS_H