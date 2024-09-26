#pragma once
#ifndef sr_ext_pos_data_H
#define sr_ext_pos_data_H

#include <cstdint> // uint16_t, uint8_t
#include <error/error.h>
#include <globals.h>

namespace egts::v1::record::subrecord
{

using frame_buffer_type = egts::v1::buffer_type;

using payload_type = egts::v1::payload_type;

using uint16_t = std::uint16_t;

using uint8_t = std::uint8_t;

using Error = egts::v1::error::Error;

using Code = egts::v1::error::Code;

// The subrecord for type EGTS_SR_EXT_POS_DATA.
//
struct SrExtPosData
{   
    // vdop - vertical dilution of precision (value multiplied by 100);
    uint16_t vertical_dilution_of_precision;

    // hdop - horizontal dilution of precision (value multiplied by 100);
    uint16_t horizontal_dilution_of_precision;

    // pdop - position dilution of precision (value multiplied by 100);
    uint16_t position_dilution_of_precision;

    // sat - number of visible satellites;
    uint8_t satellites;

    // ns - bit flags characterizing the navigation systems used. The following values (decimal) are defined:
    // 0 - system not defined;
    // 1 - GLONASS;
    // 2 - GPS;
    // 4 - Galileo;
    // 8 - Compass;
    // 16 - Beidou;
    // 32 - DORIS;
    // 64 - IRNSS;
    // 128 - QZSS.
    // Other values are reserved.
    uint16_t navigation_system;

    void parse(payload_type);

    frame_buffer_type
    buffer() const noexcept;
};

} // namespace egts::v1::record::subrecord

#endif
