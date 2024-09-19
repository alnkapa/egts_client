#pragma once
#ifndef SR_RESULT_CODE_H
#define SR_RESULT_CODE_H

#include "../../error/error.h"
#include "../subrecord.h"
#include <cstdint>

namespace egts::v1::record::subrecord
{

// The subrecord for type EGTS_SR_RESULT_CODE.
struct SrResultCode
{
    // The result of the authorization operation.
    error::Code record_status;

    error::Error parse(record_payload_type) noexcept;
    
    frame_buffer_type
    buffer() const noexcept;
};

} // namespace egts::v1::record::subrecord

#endif
