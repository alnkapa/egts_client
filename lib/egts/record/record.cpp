#include "record.h"
#include <cstddef>
#include <iterator>
#include <span>

namespace egts::v1::record
{

void
Record::parse(payload_type buffer, payload_type::iterator &ptr)
{

    auto begin = ptr;

    auto has_remaining_bytes = [&buffer, &ptr](std::size_t x)
    {
        if (ptr < buffer.begin() || ptr > buffer.end())
        {
            return false;
        }
        return static_cast<std::size_t>(std::distance(ptr, buffer.end())) >= x;
    };

    if (!has_remaining_bytes(5))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }

    m_length = static_cast<std::uint16_t>(*ptr++) |     // 0
               static_cast<std::uint16_t>(*ptr++) << 8; // 1

    if (m_length > max_frame_size)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }

    m_number = static_cast<std::uint16_t>(*ptr++) |     // 2
               static_cast<std::uint16_t>(*ptr++) << 8; // 3
    auto flag = static_cast<uint8_t>(*ptr++);           // 4

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

    if (!has_remaining_bytes(m_length))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    auto offset = std::distance(buffer.begin(), begin);
    offset += std::distance(begin, ptr);
    mp_data = buffer.subspan(offset, m_length);
    ptr += m_length;

#ifdef MY_DEBUG
    std::cout << "parse record number: " << static_cast<int>(m_number) << " source serv: " << m_source_service_type << " receive serv: " << m_recipient_service_type << " length: " << static_cast<int>(m_length) << "\ndata: " << mp_data << std::endl;
#endif
}

uint16_t
Record::record_number() const
{
    return m_number;
}

payload_type
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

buffer_type
wrapper(uint16_t record_number, ServiceType source_service_type, ServiceType recipient_service_type, buffer_type &&data)
{
    if (data.size() > max_frame_size)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    auto record_length = data.size();
    const std::size_t size = 5 + 2;
    buffer_type buffer(size, 0);
    buffer.reserve(size + record_length);
    auto ptr = buffer.begin();
    *ptr++ = static_cast<uint8_t>(record_length);          // 0
    *ptr++ = static_cast<uint8_t>(record_length >> 8);     // 1
    *ptr++ = static_cast<uint8_t>(record_number);          // 2
    *ptr++ = static_cast<uint8_t>(record_number >> 8);     // 3
    *ptr++ = 1 << 7;                                       // 4 set SSOD. We are on the client side.
    *ptr++ = static_cast<uint8_t>(source_service_type);    // 5
    *ptr++ = static_cast<uint8_t>(recipient_service_type); // 6
    buffer.insert(
        ptr,
        std::make_move_iterator(data.begin()),
        std::make_move_iterator(data.end()));

#ifdef MY_DEBUG
    std::cout << "wrapper record: " << static_cast<int>(record_number) << " source serv: " << source_service_type << " receive serv: " << recipient_service_type << " length: " << static_cast<int>(record_length) << "\ndata: " << buffer << std::endl;
#endif
    return buffer;
}

uint16_t
Record::length() const
{
    return m_length;
}

} // namespace egts::v1::record

std::ostream &
operator<<(std::ostream &os, const egts::v1::record::ServiceType &serviceType)
{
    using egts::v1::record::ServiceType;
    switch (serviceType)
    {
    case ServiceType::UNDEFINED:
        os << "UNDEFINED";
        break;
    case ServiceType::EGTS_AUTH_SERVICE:
        os << "EGTS_AUTH_SERVICE";
        break;
    case ServiceType::EGTS_TELEDATA_SERVICE:
        os << "EGTS_TELEDATA_SERVICE";
        break;
    case ServiceType::EGTS_COMMANDS_SERVICE:
        os << "EGTS_COMMANDS_SERVICE";
        break;
    case ServiceType::EGTS_FIRMWARE_SERVICE:
        os << "EGTS_FIRMWARE_SERVICE";
        break;
    case ServiceType::EGTS_ECALL_SERVICE:
        os << "EGTS_ECALL_SERVICE";
        break;
    default:
        os << "Unknown ServiceType";
        break;
    }
    return os;
}
