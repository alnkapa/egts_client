#include "sr_record_response.h"
#include <iterator>
#include <span>

namespace egts::v1::record::subrecord
{

error::Error
SRRecordResponse::parse(record_payload_type buffer) noexcept
{
    auto ptr = buffer.begin();
    if (!has_remaining_bytes(buffer, ptr, 3))
    {
        return error::Error(error::Code::EGTS_PC_INVDATALEN);
    }
    confirmed_record_number = static_cast<std::uint16_t>(*ptr++) |     // 0
                              static_cast<std::uint16_t>(*ptr++) << 8; // 1

    record_status = static_cast<error::Code>(*ptr++); // 2
    return {};
}

frame_buffer_type
SRRecordResponse::buffer() const noexcept
{
    frame_buffer_type buffer(3, 0);
    auto ptr = buffer.begin();
    *ptr++ = static_cast<std::uint8_t>(confirmed_record_number);      // 0
    *ptr++ = static_cast<std::uint8_t>(confirmed_record_number >> 8); // 1
    *ptr++ = static_cast<std::uint8_t>(record_status);                // 2
    return std::move(buffer);
}

} // namespace egts::v1::record::subrecord