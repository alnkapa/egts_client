#include "sr_result_code.h"
#include <iterator>
#include <span>

namespace egts::v1::record::subrecord
{

error::Error
SrResultCode::parse(record_payload_type buffer) noexcept
{
    auto ptr = buffer.begin();
    if (!has_remaining_bytes(buffer, ptr, 1))
    {
        return error::Error(error::Code::EGTS_PC_INVDATALEN);
    }
    record_status = static_cast<error::Code>(*ptr++); // 0
    return {};
}

frame_buffer_type
SrResultCode::buffer() const noexcept
{
    frame_buffer_type buffer(1, 0);
    auto ptr = buffer.begin();
    *ptr++ = static_cast<std::uint8_t>(record_status); // 0
    return std::move(buffer);
}

} // namespace egts::v1::record::subrecord