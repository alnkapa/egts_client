#include "sr_result_code.h"
#include <iterator>
#include <span>

namespace egts::v1::record::subrecord
{

void
SrResultCode::parse(record_payload_type buffer)
{
    auto ptr = buffer.begin();
    if (!has_remaining_bytes(buffer, ptr, 1))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    record_status = static_cast<Code>(*ptr++); // 0
}

frame_buffer_type
SrResultCode::buffer() const noexcept
{
    frame_buffer_type buffer(1, 0);
    auto ptr = buffer.begin();
    *ptr++ = static_cast<uint8_t>(record_status); // 0
    return buffer;
}

} // namespace egts::v1::record::subrecord