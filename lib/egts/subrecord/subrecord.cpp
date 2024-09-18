#include "subrecord.h"
#include <iterator>
#include <span>

namespace egts::v1::record::subrecord
{

error::Error
SubRecord::parse(record_payload_type buffer, record_payload_type::iterator &ptr) noexcept
{
    auto begin = ptr;
    
    if (!has_remaining_bytes(buffer, ptr, 3))
    {
        return error::Error(error::Code::EGTS_PC_INVDATALEN);
    }
    m_type = static_cast<Type>(*ptr++);                 // 0
    m_length = static_cast<std::uint16_t>(*ptr++) |     // 1
               static_cast<std::uint16_t>(*ptr++) << 8; // 2

    if (m_length > max_frame_size)
    {
        return error::Error(error::Code::EGTS_PC_INVDATALEN);
    }

    if (!has_remaining_bytes(buffer, ptr,m_length))
    {
        return error::Error(error::Code::EGTS_PC_INVDATALEN);
    }
    auto offset = std::distance(buffer.begin(), begin);
    offset += std::distance(begin, ptr);
    mp_data = buffer.subspan(offset, m_length);
    ptr += m_length;

    return {};
}

Type
SubRecord::type() const
{
    return m_type;
}

record_payload_type
SubRecord::data() const
{
    return mp_data;
}

frame_buffer_type
wrapper(Type type, frame_buffer_type &&data)
{
    if (data.size() > max_frame_size)
    {
        throw error::Error(error::Code::EGTS_PC_INVDATALEN);
    }
    auto record_length = data.size();

    std::size_t size = 3 + record_length;

    frame_buffer_type buffer(size, 0);
    auto ptr = buffer.begin();
    *ptr++ = static_cast<std::uint8_t>(type);               // 0
    *ptr++ = static_cast<std::uint8_t>(record_length);      // 1
    *ptr++ = static_cast<std::uint8_t>(record_length >> 8); // 2
    buffer.insert(ptr, std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
    return std::move(buffer);
}

} // namespace egts::v1::record::subrecord