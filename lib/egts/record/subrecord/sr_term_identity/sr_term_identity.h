#pragma once
#ifndef SR_TERM_IDENTITY_H
#define SR_TERM_IDENTITY_H

#include <error.h>
#include <subrecord.h>
#include <array>
#include <cstdint>

namespace egts::v1::record::subrecord
{

// The subrecord for type EGTS_SR_TERM_IDENTITY.
struct SrTermIdentity
{
    // The identifier of the mobile device (modem).
    std::array<char, 15> IMEI{};
    // The maximum size of the ASN receive buffer in bytes.
    // The size of each information packet transmitted to the ASN must not exceed this value.
    u_int16_t buffer_size{};

    error::Error parse(record_payload_type) noexcept;
    
    frame_buffer_type
    buffer() const noexcept;
};

} // namespace egts::v1::record::subrecord

#endif
