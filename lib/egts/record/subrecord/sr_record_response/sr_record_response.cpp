#include "sr_record_response.h"

namespace egts::v1::record::subrecord
{

void
SRRecordResponse::parse(payload_type buffer)
{
    auto ptr = buffer.begin();
    if (!has_remaining_bytes(buffer, ptr, 3))
    {
        throw(error::Error(error::Code::EGTS_PC_INVDATALEN));
    }

    confirmed_record_number = static_cast<std::uint16_t>(*ptr++) |     // 0
                              static_cast<std::uint16_t>(*ptr++) << 8; // 1
                                                                       //
    record_status = static_cast<error::Code>(*ptr++);                  // 2
}

frame_buffer_type
SRRecordResponse::buffer() const noexcept
{
    frame_buffer_type buffer(3, 0);
    auto ptr = buffer.begin();
    *ptr++ = static_cast<std::uint8_t>(confirmed_record_number);      // 0
    *ptr++ = static_cast<std::uint8_t>(confirmed_record_number >> 8); // 1
    *ptr++ = static_cast<std::uint8_t>(record_status);                // 2

    return buffer;
}

SRRecordResponse::SRRecordResponse(){};

SRRecordResponse::SRRecordResponse(uint16_t confirmed_record_number, Code record_status)
    : confirmed_record_number(confirmed_record_number), record_status(record_status){};

} // namespace egts::v1::record::subrecord