#include "sr_ext_pos_data.h"
#include <cstddef>
#include <span>

namespace egts::v1::record::subrecord
{

void
SrExtPosData::parse(payload_type buffer)
{
    auto ptr = buffer.begin();
    if (!has_remaining_bytes(buffer, ptr, 1))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    // -[3] nsfe[1] sfe[1] pfe[1] hfe[1] vfe[1]
    // nsfe - (NavigationSystemFieldExists)
    // Indicates the presence of data about the types of navigation satellite systems used:
    // 1 - ns field is transmitted;
    // 0 - not transmitted.
    // sfe - (SatellitesFieldExists) indicates the presence of data about the current number
    // of visible satellites (sat) and the type of navigation satellite system (ns) used:
    // 1 - sat and ns fields are transmitted;
    // 0 - not transmitted.
    // pfe - (PDOP FieldExists) indicates the presence of the PDOP field:
    // 1 - PDOP field is transmitted;
    // 0 - not transmitted.
    // hfe - (HDOP FieldExists) indicates the presence of the HDOP field:
    // 1 - HDOP field is transmitted;
    // 0 - not transmitted.
    // vfe - (VDOP FieldExists) indicates the presence of the VDOP field:
    // 1 - VDOP field is transmitted;
    // 0 - not transmitted.
    auto flag = static_cast<uint8_t>(*ptr++); // 0

    if (flag & 0x01) // VDOP
    {
        if (!has_remaining_bytes(buffer, ptr, 2))
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        vertical_dilution_of_precision = static_cast<uint16_t>(*ptr++) |
                                         static_cast<uint16_t>(*ptr++) << 8;
    }

    if (flag & 0x02) // HDOP
    {
        if (!has_remaining_bytes(buffer, ptr, 2))
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        horizontal_dilution_of_precision = static_cast<uint16_t>(*ptr++) |
                                           static_cast<uint16_t>(*ptr++) << 8;
    }

    if (flag & 0x04) // PDOP
    {
        if (!has_remaining_bytes(buffer, ptr, 2))
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        position_dilution_of_precision = static_cast<uint16_t>(*ptr++) |
                                         static_cast<uint16_t>(*ptr++) << 8;
    }

    if (flag & 0x10) // SAT
    {
        if (!has_remaining_bytes(buffer, ptr, 1))
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        satellites = static_cast<uint8_t>(*ptr++);
    }

    if (flag & 0x20) // NS
    {
        if (!has_remaining_bytes(buffer, ptr, 1))
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        navigation_system = static_cast<uint16_t>(*ptr++) |
                            static_cast<uint16_t>(*ptr++) << 8;
    }
}

frame_buffer_type
SrExtPosData::buffer() const noexcept
{
    size_t size = 1;
    if (vertical_dilution_of_precision != 0)
    {
        size += 2;
    }
    if (horizontal_dilution_of_precision != 0)
    {
        size += 2;
    }
    if (position_dilution_of_precision != 0)
    {
        size += 2;
    }
    if (satellites != 0)
    {
        size += 1;
    }
    if (navigation_system != 0)
    {
        size += 2;
    }

    frame_buffer_type buffer(size, 0);
    auto ptr = buffer.begin()+1;
    auto flag = buffer.begin();

    if (vertical_dilution_of_precision != 0)
    {
        *flag |= 0x01; // VDOP
        *ptr++ = static_cast<uint8_t>(vertical_dilution_of_precision);
        *ptr++ = static_cast<uint8_t>(vertical_dilution_of_precision >> 8);
    }

    if (horizontal_dilution_of_precision != 0)
    {
        *flag |= 0x02; // HDOP
        *ptr++ = static_cast<uint8_t>(horizontal_dilution_of_precision);
        *ptr++ = static_cast<uint8_t>(horizontal_dilution_of_precision >> 8);
    }

    if (position_dilution_of_precision != 0)
    {
        *flag |= 0x04; // PDOP
        *ptr++ = static_cast<uint8_t>(position_dilution_of_precision);
        *ptr++ = static_cast<uint8_t>(position_dilution_of_precision >> 8);
    }

    if (satellites != 0)
    {
        *flag |= 0x10; // SAT
        *ptr++ = static_cast<uint8_t>(satellites);
    }

    if (navigation_system != 0)
    {
        *flag |= 0x20; // NS
        *ptr++ = static_cast<uint8_t>(navigation_system);
        *ptr++ = static_cast<uint8_t>(navigation_system >> 8);
    }
    return buffer;
}

} // namespace egts::v1::record::subrecord