#pragma once
#ifndef SR_RECORD_RESPONSE_H
#define SR_RECORD_RESPONSE_H

#include "../../error/error.h"
#include "../subrecord.h"
#include <cstdint>

namespace egts::v1::record::subrecord
{

// The subrecord for type EGTS_SR_RECORD_RESPONSE.
struct SRRecordResponse
{
    // The number of the confirmed record (the value of the RN field from the processed record).
    uint16_t confirmed_record_number;
    // The status of the record processing.
    error::Code record_status;

    error::Error parse(record_payload_type) noexcept;
    frame_buffer_type
    buffer() const noexcept;
};

} // namespace egts::v1::record::subrecord

#endif