#include "record.h"
#include <iterator>
#include <span>

namespace egts::v1::record
{

void
Record::parse(record_payload_type buffer, record_payload_type::iterator &ptr)
{
    auto begin = ptr;

    auto has_remaining_bytes = [&buffer, &ptr](std::size_t x)
    {
        if (ptr < buffer.begin() || ptr > buffer.end())
        {
            return false;
        }
        return std::distance(ptr, buffer.end()) >= x;
    };

    if (!has_remaining_bytes(5))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }

    m_record_length = static_cast<std::uint16_t>(*ptr++) |     // 0
                      static_cast<std::uint16_t>(*ptr++) << 8; // 1

    if (m_record_length > max_frame_size)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }

    m_record_number = static_cast<std::uint16_t>(*ptr++) |     // 2
                      static_cast<std::uint16_t>(*ptr++) << 8; // 3
    auto flag = static_cast<uint8_t>(*ptr++);                  // 4

    // NO NEED
    // bool ssod = flag & 0x80 != 0;

    // NO NEED
    // bool rsod = flag & 0x40 != 0;

    // NO NEED
    // bool grp = flag & 0x20 != 0;

    // NO NEED
    // uint8_t rpp = (flag & 0x16) >> 3;

    if ((flag & 0x01) != 0) // OBFE
    {
        if (!has_remaining_bytes(4))
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        // NO NEED Object Identifier
        ptr += 4; // UINT
    }

    if ((flag & 0x02) != 0) // EVFE
    {
        if (!has_remaining_bytes(4))
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        // NO NEED Event Identifier
        ptr += 4; // UINT
    }

    if ((flag & 0x04) != 0) // tmfe
    {
        if (!has_remaining_bytes(4))
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        // NO NEED TIME
        ptr += 4; // UINT
    }

    if (!has_remaining_bytes(2))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    m_source_service_type = static_cast<ServiceType>(*ptr++);
    m_recipient_service_type = static_cast<ServiceType>(*ptr++);

    if (!has_remaining_bytes(m_record_length))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    auto offset = std::distance(buffer.begin(), begin);
    offset += std::distance(begin, ptr);
    mp_data = buffer.subspan(offset, m_record_length);
    ptr += m_record_length;
}

uint16_t
Record::record_number() const
{
    return m_record_number;
}

record_payload_type
Record::data() const
{
    return mp_data;
}

ServiceType
Record::source_service_type() const
{
    return m_source_service_type;
}

ServiceType
Record::recipient_service_type() const
{
    return m_recipient_service_type;
}

frame_buffer_type
wrapper(uint16_t record_number, ServiceType source_service_type, ServiceType recipient_service_type, frame_buffer_type &&data)
{
    if (data.size() > max_frame_size)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    auto record_length = data.size();
    std::size_t size = 5 + 2 + record_length;
    frame_buffer_type buffer(size, 0);
    auto ptr = buffer.begin();
    *ptr++ = static_cast<uint8_t>(record_length);          // 0
    *ptr++ = static_cast<uint8_t>(record_length >> 8);     // 1
    *ptr++ = static_cast<uint8_t>(record_number);          // 2
    *ptr++ = static_cast<uint8_t>(record_number >> 8);     // 3
    *ptr++ = 1 << 7;                                       // 4 set SSOD. We are on the client side.
    *ptr++ = static_cast<uint8_t>(source_service_type);    // 5
    *ptr++ = static_cast<uint8_t>(recipient_service_type); // 6
    buffer.insert(ptr, std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
    return buffer;
}

} // namespace egts::v1::record
