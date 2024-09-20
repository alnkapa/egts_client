#pragma once
#ifndef SR_RECORD_RESPONSE_H
#define SR_RECORD_RESPONSE_H

#include <error.h>
#include <subrecord.h>
#include <cstdint>

namespace egts::v1::record::subrecord
{

// The subrecord for type EGTS_SR_RECORD_RESPONSE.
struct SRRecordResponse : public SubRec
{
    // The number of the confirmed record (the value of the RN field from the processed record).
    uint16_t confirmed_record_number;
    // The status of the record processing.
    error::Code record_status;

    void parse(record_payload_type) override;

    frame_buffer_type
    buffer() const noexcept override;
};

} // namespace egts::v1::record::subrecord

#endif