#pragma once
#ifndef SR_RESULT_CODE_H
#define SR_RESULT_CODE_H

#include <cstdint> // uint16_t, uint8_t, uint32_t
#include <error/error.h>
#include <globals.h>

namespace egts::v1::record::subrecord
{

using frame_buffer_type = egts::v1::buffer_type;

using payload_type = egts::v1::payload_type;

using uint16_t = std::uint16_t;

using uint32_t = std::uint16_t;

using uint8_t = std::uint8_t;

using Error = egts::v1::error::Error;

using Code = egts::v1::error::Code;

// The subrecord for type EGTS_SR_POS_DATA
struct SrPosData
{
    // NTM - navigation time
    //
    // number of seconds since 00:00:00 01.01.2010 UTC
    uint32_t navigation_time{0};

    // LAT - latitude in absolute value
    //
    // degrees/90 · 0xFFFFFFFF and taken as the integer part;
    uint32_t latitude{0};

    // LONG - longitude in absolute value
    //
    // degrees/180 · 0xFFFFFFFF and taken as the integer part;
    uint32_t longitude{0};

    // Defines additional parameters of the navigation message;
    //
    // ALTE - bit flag indicating the presence of the ALT field in the subrecord:
    // 1 - ALT field is transmitted;
    // 0 - not transmitted;
    // LOHS - bit flag indicating the hemisphere of longitude:
    // 0 - eastern longitude;
    // 1 - western longitude;
    // LAHS - bit flag indicating the hemisphere of latitude:
    // 0 - northern latitude;
    // 1 - southern latitude;
    // MV - bit flag indicating movement:
    // 1 - moving;
    // 0 - vehicle is stationary;
    // BB - bit flag indicating data sent from memory ("black box"):
    // 0 - current data;
    // 1 - data from memory ("black box");
    // FIX - bit field, type of coordinate determination:
    // 0 - 2D fix;
    // 1 - 3D fix;
    // CS - bit field, type of coordinate system used:
    // 0 - WGS-84 coordinate system;
    // 1 - national geocentric coordinate system (PZ-90.02);
    // VLD - bit flag indicating the validity of coordinate data:
    // 1 - data is valid;
    // 0 - data is invalid;
    // ALTS - (AltitudeSign) bit flag indicating height relative to sea level and is meaningful only when the ALTE flag is set:
    // 0 - point is above sea level;
    // 1 - below sea level;
    uint8_t flags{0};

    // SPD - speed in km/h with a resolution of 0.1 km/h
    //
    // (uses 14 least significant bits);
    //
    // DIRH - (Direction the Highest bit) the highest bit (8) of the DIR parameter;
    uint16_t speed{0};

    // DIR - direction of movement.
    //
    // Defined as the angle in degrees measured clockwise from
    // the north direction of the geographic meridian to the direction
    // of movement at the measurement point
    // (the highest bit is additionally in the DIRH field);
    uint8_t direction{0};

    // ODM - distance traveled (odometer) in km,
    //
    // with a resolution of 0.1 km;
    // number of hundreds of meters
    uint32_t odometer{0};

    // DIN - bit flags indicating the state of discrete inputs 1 ... 8
    //
    // (if the bit is 1, the corresponding input is active, if 0, it is inactive).
    uint8_t digital_inputs{0};

    // SRC - indicates the source (event) that initiated the transmission of this navigation information
    //
    // (information is presented in Table B.3);
    uint8_t source{0};

    // ALT - height above sea level, m
    //
    // (optional parameter, presence of which is determined by the ALTE bit flag);
    uint32_t altitude{0};

    // SRCD - data characterizing the source (event) from the SRC field.
    //
    // The presence and interpretation of this field's value is determined by the SRC field.
    uint16_t source_data{0};

    void parse(payload_type);

    frame_buffer_type
    buffer() const noexcept;
};

} // namespace egts::v1::record::subrecord

#endif
