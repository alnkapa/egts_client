#pragma once
#ifndef SR_RESULT_CODE_H
#define SR_RESULT_CODE_H

#include <cstdint> // uint16_t, uint8_t
#include <error/error.h>
#include <globals.h>

namespace egts::v1::record::subrecord
{

using buffer_type = egts::v1::buffer_type;

using payload_type = egts::v1::payload_type;

using uint16_t = std::uint16_t;

using uint8_t = std::uint8_t;

using Error = egts::v1::error::Error;

using Code = egts::v1::error::Code;

// The subrecord for type EGTS_SR_RESULT_CODE.
struct SrResultCode
{
    // The result of the authorization operation.
    Code record_status;

    void parse(payload_type);

    buffer_type
    buffer() const noexcept;

    SrResultCode();

    SrResultCode(payload_type);

    Error
    error() const
    {
        return Error(record_status);
    }
};

} // namespace egts::v1::record::subrecord

#endif
