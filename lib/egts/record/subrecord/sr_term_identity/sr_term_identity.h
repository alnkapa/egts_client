#pragma once
#ifndef SR_TERM_IDENTITY_H
#define SR_TERM_IDENTITY_H

#include <cstdint> // uint16_t, uint8_t
#include <error/error.h>
#include <globals.h>
#include <array>

namespace egts::v1::record::subrecord
{

using frame_buffer_type = egts::v1::frame_buffer_type;

using record_payload_type = egts::v1::record_payload_type;

using uint16_t = std::uint16_t;

using uint8_t = std::uint8_t;

using Error = egts::v1::error::Error;

using Code = egts::v1::error::Code;

// The subrecord for type EGTS_SR_TERM_IDENTITY.
struct SrTermIdentity
{
    // The identifier of the mobile device (modem).
    std::array<char, 15> IMEI{};
    // The maximum size of the ASN receive buffer in bytes.
    // The size of each information packet transmitted to the ASN must not exceed this value.
    uint16_t buffer_size{};

    void parse(record_payload_type);

    frame_buffer_type
    buffer() const noexcept;
};

} // namespace egts::v1::record::subrecord

#endif
