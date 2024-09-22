#pragma once
#ifndef SR_RECORD_RESPONSE_H
#define SR_RECORD_RESPONSE_H

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

// The subrecord for type EGTS_SR_RECORD_RESPONSE.
struct SRRecordResponse
{
    // The number of the confirmed record (the value of the RN field from the processed record).
    uint16_t confirmed_record_number;
    // The status of the record processing.
    Code record_status;

    void parse(payload_type);

    frame_buffer_type
    buffer() const noexcept;
};

} // namespace egts::v1::record::subrecord

#endif