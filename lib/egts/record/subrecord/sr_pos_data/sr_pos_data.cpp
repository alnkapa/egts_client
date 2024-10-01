#include "sr_pos_data.h"
#include <span>

namespace egts::v1::record::subrecord
{

void
SrPosData::parse(payload_type buffer)
{
    auto ptr = buffer.begin();
    if (!has_remaining_bytes(buffer, ptr, 21))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    navigation_time = static_cast<uint32_t>(*ptr++) |       // 0
                      static_cast<uint32_t>(*ptr++) << 8 |  // 1
                      static_cast<uint32_t>(*ptr++) << 16 | // 2
                      static_cast<uint32_t>(*ptr++) << 24;  // 3
                                                            //
    latitude = static_cast<uint32_t>(*ptr++) |              // 4
               static_cast<uint32_t>(*ptr++) << 8 |         // 5
               static_cast<uint32_t>(*ptr++) << 16 |        // 6
               static_cast<uint32_t>(*ptr++) << 24;         // 7
                                                            //
    longitude = static_cast<uint32_t>(*ptr++) |             // 8
                static_cast<uint32_t>(*ptr++) << 8 |        // 9
                static_cast<uint32_t>(*ptr++) << 16 |       // 10
                static_cast<uint32_t>(*ptr++) << 24;        // 11
                                                            //
    flags = static_cast<uint8_t>(*ptr++);                   // 12
                                                            //
    speed = static_cast<uint16_t>(*ptr++) |                 // 13
            static_cast<uint16_t>(*ptr++ & 0x3f) << 8;      // 14
                                                            //
    direction = static_cast<uint8_t>(*ptr++);               // 15
                                                            //
    odometer = static_cast<uint32_t>(*ptr++) |              // 16
               static_cast<uint32_t>(*ptr++) << 8 |         // 17
               static_cast<uint32_t>(*ptr++) << 16;         // 18
                                                            //
    digital_inputs = static_cast<uint8_t>(*ptr++);          // 19
                                                            //
    source = static_cast<uint8_t>(*ptr++);                  // 20

    if ((flags & 0x80) != 0) // ALTH
    {
        if (!has_remaining_bytes(buffer, ptr, 3))
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        altitude = static_cast<uint32_t>(*ptr++) |
                   static_cast<uint32_t>(*ptr++) << 8 |
                   static_cast<uint32_t>(*ptr++) << 16;
    }

    if (has_remaining_bytes(buffer, ptr, 2))
    {
        source_data = static_cast<uint16_t>(*ptr++) |
                      static_cast<uint16_t>(*ptr++) << 8;
    }
}

buffer_type
SrPosData::buffer() const noexcept
{
    size_t buffer_size = 21;

    if ((flags & 0x80) != 0) // ALTH
    {
        buffer_size += 3;
    }
    if (source_data)
    {
        buffer_size += 2;
    }

    buffer_type buffer(buffer_size, 0);
    auto ptr = buffer.begin();

    *ptr++ = static_cast<uint8_t>(navigation_time);       // 0
    *ptr++ = static_cast<uint8_t>(navigation_time >> 8);  // 1
    *ptr++ = static_cast<uint8_t>(navigation_time >> 16); // 2
    *ptr++ = static_cast<uint8_t>(navigation_time >> 24); // 3
                                                          //
    *ptr++ = static_cast<uint8_t>(latitude);              // 4
    *ptr++ = static_cast<uint8_t>(latitude >> 8);         // 5
    *ptr++ = static_cast<uint8_t>(latitude >> 16);        // 6
    *ptr++ = static_cast<uint8_t>(latitude >> 24);        // 7
                                                          //
    *ptr++ = static_cast<uint8_t>(longitude);             // 8
    *ptr++ = static_cast<uint8_t>(longitude >> 8);        // 9
    *ptr++ = static_cast<uint8_t>(longitude >> 16);       // 10
    *ptr++ = static_cast<uint8_t>(longitude >> 24);       // 11
                                                          //
    *ptr++ = static_cast<uint8_t>(flags);                 // 12
                                                          //
    *ptr++ = static_cast<uint8_t>(speed);                 // 13
    *ptr++ = static_cast<uint8_t>((speed >> 8) & 0x3f);   // 14
                                                          //
    *ptr++ = static_cast<uint8_t>(direction);             // 15
                                                          //
    *ptr++ = static_cast<uint8_t>(odometer);              // 16
    *ptr++ = static_cast<uint8_t>(odometer >> 8);         // 17
    *ptr++ = static_cast<uint8_t>(odometer >> 16);        // 18
                                                          //
    *ptr++ = static_cast<uint8_t>(digital_inputs);        // 19
                                                          //
    *ptr++ = static_cast<uint8_t>(source);                // 20

    if ((flags & 0x80) != 0)
    {
        *ptr++ = static_cast<uint8_t>(altitude);
        *ptr++ = static_cast<uint8_t>(altitude >> 8);
        *ptr++ = static_cast<uint8_t>(altitude >> 16);
    }

    if (source_data)
    {
        *ptr++ = static_cast<uint8_t>(source_data);
        *ptr++ = static_cast<uint8_t>(source_data >> 8);
    }
    return buffer;
}

} // namespace egts::v1::record::subrecord