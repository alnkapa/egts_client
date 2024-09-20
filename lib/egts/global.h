// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <span>

using frame_buffer_type = std::vector<unsigned char>;

using record_payload_type = std::span<const unsigned char>;

#endif // GLOBALS_H