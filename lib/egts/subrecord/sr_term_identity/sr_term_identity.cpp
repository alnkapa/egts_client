#include "sr_term_identity.h"
#include <algorithm>
#include <iterator>
#include <span>

namespace egts::v1::record::subrecord
{

error::Error
SrTermIdentity::parse(record_payload_type buffer) noexcept
{
    auto ptr = buffer.begin();
    if (!has_remaining_bytes(buffer, ptr, 5))
    {
        return error::Error(error::Code::EGTS_PC_INVDATALEN);
    }
    // skip Terminal Identifier
    ptr += 4;
    uint8_t flag = static_cast<uint8_t>(*ptr++); // 5
    if ((flag & 0x01) != 0)                      // HDIDE
    {
        if (!has_remaining_bytes(buffer, ptr, 2))
        {
            return error::Error(error::Code::EGTS_PC_INVDATALEN);
        }
        // NO NEED Home Dispatcher Identifier
        ptr += 2; // USHORT
    }

    if ((flag & 0x02) != 0) // IMEIE
    {
        if (!has_remaining_bytes(buffer, ptr, 15))
        {
            return error::Error(error::Code::EGTS_PC_INVDATALEN);
        }
        // IMEI
        std::copy_n(ptr, 15, IMEI.begin());
        ptr += 15; // CHAR[15]
    }

    if ((flag & 0x04) != 0) // IMSIE
    {
        if (!has_remaining_bytes(buffer, ptr, 16))
        {
            return error::Error(error::Code::EGTS_PC_INVDATALEN);
        }
        // NO NEED International Mobile Subscriber Identity
        ptr += 16; // CHAR[16]
    }

    if ((flag & 0x08) != 0) // LNGCE
    {
        if (!has_remaining_bytes(buffer, ptr, 3))
        {
            return error::Error(error::Code::EGTS_PC_INVDATALEN);
        }
        // NO NEED Language Code
        ptr += 3; // CHAR[3]
    }

    if ((flag & 0x20) != 0) // NIDE
    {
        if (!has_remaining_bytes(buffer, ptr, 3))
        {
            return error::Error(error::Code::EGTS_PC_INVDATALEN);
        }
        // NO NEED Network Identifier
        ptr += 3; // CHAR[3]
    }

    if ((flag & 0x40) != 0) // BSE
    {
        if (!has_remaining_bytes(buffer, ptr, 2))
        {
            return error::Error(error::Code::EGTS_PC_INVDATALEN);
        }
        // Buffer Size
        buffer_size = static_cast<std::uint16_t>(*ptr++) |     // 0
                      static_cast<std::uint16_t>(*ptr++) << 8; // 1
    }

    if ((flag & 0x80) != 0) // MNE
    {
        if (!has_remaining_bytes(buffer, ptr, 15))
        {
            return error::Error(error::Code::EGTS_PC_INVDATALEN);
        }
        // NO NEED Mobile Station Integrated Services Digital Network Number
        ptr += 15; // CHAR[15]
    }

    return {};
}

frame_buffer_type
SrTermIdentity::buffer() const noexcept
{
    frame_buffer_type buffer(5 + 15 + 2, 0);
    auto ptr = buffer.begin();
    ptr += 4;                                             // skip TID
    *ptr++ = static_cast<std::uint8_t>(0x02 | 0x40);      // set flag IMEIE and BSE
    std::copy(IMEI.begin(), IMEI.end(), ptr);             // add IMEI
    ptr += 15;                                            //
    *ptr++ = static_cast<std::uint8_t>(buffer_size);      // add BS
    *ptr++ = static_cast<std::uint8_t>(buffer_size >> 8); // add BS
    return std::move(buffer);
}

} // namespace egts::v1::record::subrecord