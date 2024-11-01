#include "transport.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <utility>

namespace egts::v1::transport
{

Packet::Packet()
{
}

Packet::Packet(buffer_type &&in)
{
    set_frame(std::move(in));
}

Packet::Packet(uint16_t packet_identifier, buffer_type &&in)
    : Packet(std::move(in))
{
    m_packet_identifier = packet_identifier;
}

void
Packet::identifier(uint16_t packet_identifier)
{
    m_packet_identifier = packet_identifier;
}

uint16_t
Packet::identifier() const
{
    return m_packet_identifier;
}

bool
Packet::is_response() const
{
    return m_packet_type == Type::EGTS_PT_RESPONSE;
}

std::pair<uint16_t, Code>
Packet::response() const
{
    return std::pair<uint16_t, Code>(m_response_packet_identifier, m_processing_result);
}

uint16_t
Packet::frame_data_length() const
{
    return m_frame_data_length;
}

Packet
Packet::make_response(const Code &processing_result)
{
    Packet response{};
    response.m_response_packet_identifier = m_packet_identifier;
    response.m_processing_result = processing_result;
    response.m_packet_type = Type::EGTS_PT_RESPONSE;
    return response;
}

void
Packet::parse_frame(buffer_type &&buffer)
{
    // test size of frame size
    if (buffer.size() != static_cast<std::size_t>(m_frame_data_length + crc_data_length))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    // test crc
    uint16_t crc16_val = egts::v1::crc16(buffer.begin(), buffer.end() - crc_data_length);
    uint16_t got_crc16_val = static_cast<uint16_t>(buffer[m_frame_data_length]) |
                             static_cast<uint16_t>(buffer[m_frame_data_length + 1]) << 8;

    if (crc16_val != got_crc16_val)
    {
        throw Error(Code::EGTS_PC_DATACRC_ERROR);
    }
    // cut crc value
    buffer.resize(m_frame_data_length);
    // parse response
    if (is_response())
    {
        if (m_frame_data_length < response_length)
        {
            throw Error(Code::EGTS_PC_INC_DATAFORM);
        }
        m_response_packet_identifier = static_cast<uint16_t>(buffer[0]) |
                                       static_cast<uint16_t>(buffer[1]) << 8;
        m_processing_result = static_cast<Code>(buffer[2]);
        buffer.erase(buffer.begin(), buffer.begin() + response_length);
    }
    m_frame_data_length = buffer.size();
    mp_data = std::move(buffer);
}

void
Packet::set_frame(buffer_type &&buffer) noexcept
{
    mp_data = std::move(buffer);
    m_frame_data_length = mp_data.size();
}

payload_type
Packet::get_frame() const noexcept
{
    return payload_type(mp_data.cbegin(), mp_data.cend());
}

buffer_type
Packet::frame_to_buffer() const
{
    if (m_frame_data_length == 0 && !is_response())
    {
        return buffer_type();
    }

    uint16_t size = m_frame_data_length +  static_cast<uint16_t>(crc_data_length);
    if (is_response())
    {
        size += response_length;
    }

    buffer_type ret(size);
    auto ptr = ret.begin();

    if (is_response())
    {
        *ptr++ = static_cast<uint8_t>(m_response_packet_identifier);      // 0
        *ptr++ = static_cast<uint8_t>(m_response_packet_identifier >> 8); // 1
        *ptr++ = static_cast<uint8_t>(m_processing_result);               // 2
    }

    if (m_frame_data_length != 0)
    {
        std::copy(mp_data.begin(), mp_data.end(), ptr);
        ptr += m_frame_data_length;
    }

    uint16_t crc16_val = egts::v1::crc16(ret.begin(), ptr);
    *ptr++ = static_cast<uint8_t>(crc16_val);
    *ptr++ = static_cast<uint8_t>(crc16_val >> 8);

    return ret;
}

void
Packet::parse_header(const header_buffer_type &head)
{
    // test protocol_version and PRF
    if (head[0] != protocol_version || (head[2] & 0xC0) != 0)
    {
        throw Error(Code::EGTS_PC_UNS_PROTOCOL);
    }
    // test rte
    if ((head[2] & 0x20) != 0)
    {
        throw Error(Code::EGTS_PC_ROUTE_DENIED);
    }
    // test ENA
    if ((head[2] & 0x18) != 0)
    {
        throw Error(Code::EGTS_PC_DECRYPT_ERROR);
    }
    // test CMP
    if ((head[2] & 0x04) != 0)
    {
        throw Error(Code::EGTS_PC_INC_DATAFORM);
    }
    // test HL
    if (head[3] != header_length)
    {
        throw Error(Code::EGTS_PC_INC_HEADERFORM);
    }

    // read frame data length
    m_frame_data_length = static_cast<uint16_t>(head[5]) |
                          static_cast<uint16_t>(head[6]) << 8;
    if (m_frame_data_length > max_frame_size)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }

    // read packet identifier
    m_packet_identifier = static_cast<uint16_t>(head[7]) |
                          static_cast<uint16_t>(head[8]) << 8;

    // read packet identifier
    if (head[9] > static_cast<uint8_t>(Type::EGTS_PT_APPDATA))
    {
        throw Error(Code::EGTS_PC_UNS_TYPE);
    }

    m_packet_type = static_cast<Type>(head[9]);
    // test crc
    if (head[header_length - 1] != egts::v1::crc8(head.begin(), head.end() - crc_header_length))
    {
        throw Error(Code::EGTS_PC_HEADERCRC_ERROR);
    }

    if (is_response() && m_frame_data_length < response_length)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
}

header_buffer_type
Packet::header_to_buffer() const noexcept
{
    std::uint16_t frame_data{m_frame_data_length};
    if (is_response())
    {
        frame_data += response_length;
    }
    // make packet header
    header_buffer_type ret{
        protocol_version,                               // 0
        security_key_id,                                // 1
        flag,                                           // 2
        header_length,                                  // 3
        header_encoding,                                // 4
        static_cast<uint8_t>(frame_data),               // 5
        static_cast<uint8_t>(frame_data >> 8),          // 6
        static_cast<uint8_t>(m_packet_identifier),      // 7
        static_cast<uint8_t>(m_packet_identifier >> 8), // 8
        static_cast<uint8_t>(m_packet_type),            // 9
    };
    ret[header_length - 1] = egts::v1::crc8(ret.begin(), ret.end() - crc_header_length); // 10
    return ret;
}

buffer_type
Packet::buffer() const noexcept
{
    buffer_type ret(header_length);
    auto h = header_to_buffer();
    std::copy(h.begin(), h.end(), ret.begin());
    auto f = frame_to_buffer();
    if (!f.empty())
    {
        ret.resize(h.size() + f.size());
        std::copy(f.begin(), f.end(), ret.begin() + h.size());
    }
    return ret;
}

bool
Packet::operator==(const Packet &other) const
{
    return m_packet_identifier == other.m_packet_identifier;
}

} // namespace egts::v1::transport

std::ostream &
operator<<(std::ostream &os, const egts::v1::transport::Type &type)
{
    using egts::v1::transport::Type;
    switch (type)
    {
    case Type::EGTS_PT_RESPONSE:
        os << "EGTS_PT_RESPONSE";
        break;
    case Type::EGTS_PT_APPDATA:
        os << "EGTS_PT_APPDATA";
        break;
    case Type::EGTS_PT_SIGNED_APPDATA:
        os << "EGTS_PT_SIGNED_APPDATA";
        break;
    default:
        os << "Unknown Type";
        break;
    }
    return os;
}
