#include "transport.h"
#include "crc.h"
#include "error.h"
#include <algorithm>
#include <cassert>
#include <iterator>
namespace egts::v1::transport
{

Packet::Packet() {}

Packet::~Packet() {}

void
Packet::identifier(uint16_t packet_identifier)
{
    m_packet_identifier = packet_identifier;
}

uint16_t
Packet::identifier()
{
    return m_packet_identifier;
}

bool
Packet::is_response() const
{
    return m_packet_type == Type::EGTS_PT_RESPONSE;
}

std::pair<uint16_t, error::Error>
Packet::response() const
{
    return std::pair<uint16_t, error::Error>();
}

uint16_t
Packet::frame_data_length() const
{
    return m_frame_data_length;
}

Packet
Packet::make_response(const egts::v1::error::Error &processing_result)
{
    Packet response = *this;
    response.m_response_packet_identifier = m_packet_identifier;
    response.m_processing_result = processing_result;
    response.m_packet_type = Type::EGTS_PT_RESPONSE;
    return response;
}

using Error = egts::v1::error::Error;
using Code = egts::v1::error::Code;

Error
Packet::parse_frame(frame_buffer_type &&buffer) noexcept
{
    // test size of frame size
    if (buffer.size() != m_frame_data_length + crc_data_length)
    {
        return Error(Code::EGTS_PC_INVDATALEN);
    }
    // test crc
    uint16_t crc16_val = egts::v1::crc16(buffer.begin(), buffer.end() - crc_data_length);
    uint16_t got_crc16_val = static_cast<std::uint16_t>(buffer[m_frame_data_length]) |
                             static_cast<std::uint16_t>(buffer[m_frame_data_length + 1]) << 8;

    if (crc16_val != got_crc16_val)
    {
        return Error(Code::EGTS_PC_DATACRC_ERROR);
    }
    // cut crc value
    buffer.resize(m_frame_data_length);
    // parse response
    if (is_response())
    {
        if (m_frame_data_length < response_length)
        {
            return Error(Code::EGTS_PC_INC_DATAFORM);
        }
        m_response_packet_identifier = static_cast<std::uint16_t>(buffer[0]) |
                                       static_cast<std::uint16_t>(buffer[1]) << 8;
        m_processing_result = Error(static_cast<Code>(buffer[2]));
        buffer.erase(buffer.begin(), buffer.begin() + response_length);
    }
    m_frame_data_length = buffer.size();
    mp_data = std::move(buffer);
    return {};
}

frame_buffer_type
Packet::frame() const noexcept
{
    if (m_frame_data_length == 0 && !is_response())
    {
        return {};
    }

    auto size = m_frame_data_length + crc_data_length;
    if (is_response())
    {
        size += response_length;
    }

    frame_buffer_type ret(size);
    auto ptr = ret.begin();

    if (is_response())
    {
        *ptr++ = static_cast<std::uint8_t>(m_response_packet_identifier);      // 0
        *ptr++ = static_cast<std::uint8_t>(m_response_packet_identifier >> 8); // 1
        *ptr++ = static_cast<std::uint8_t>(m_processing_result);               // 2
    }

    if (m_frame_data_length != 0)
    {
        std::copy(mp_data.begin(), mp_data.end(), ptr);
        ptr += m_frame_data_length;
    }

    uint16_t crc16_val = egts::v1::crc16(ret.begin(), ptr);
    *ptr++ = static_cast<std::uint8_t>(crc16_val);
    *ptr++ = static_cast<std::uint8_t>(crc16_val >> 8);

    return ret;
}

Error
Packet::parse_header(const std::array<unsigned char, header_length> &head) noexcept
{
    // test protocol_version and PRF
    if (head[0] != protocol_version || (head[2] & 0xC0) != 0)
    {
        return Error(Code::EGTS_PC_UNS_PROTOCOL);
    }
    // test rte
    if ((head[2] & 0x20) != 0)
    {
        return Error(Code::EGTS_PC_ROUTE_DENIED);
    }
    // test ENA
    if ((head[2] & 0x18) != 0)
    {
        return Error(Code::EGTS_PC_DECRYPT_ERROR);
    }
    // test CMP
    if ((head[2] & 0x04) != 0)
    {
        return Error(Code::EGTS_PC_INC_DATAFORM);
    }
    // test HL
    if (head[3] != header_length)
    {
        return Error(Code::EGTS_PC_INC_HEADERFORM);
    }
    // read packet identifier
    if (head[9] > static_cast<std::uint8_t>(Type::EGTS_PT_APPDATA))
    {
        return Error(Code::EGTS_PC_UNS_TYPE);
    }
    m_packet_type = static_cast<Type>(head[9]);
    // test crc
    if (head[header_length - 1] != egts::v1::crc8(head.begin(), head.end() - crc_header_length))
    {
        return Error(Code::EGTS_PC_HEADERCRC_ERROR);
    }
    // read frame data length
    m_frame_data_length = static_cast<std::uint16_t>(head[5]) |
                          static_cast<std::uint16_t>(head[6]) << 8;
    if (m_frame_data_length > max_frame_size)
    {
        return Error(Code::EGTS_PC_INVDATALEN);
    }
    if (is_response() && m_frame_data_length < response_length)
    {
        return Error(Code::EGTS_PC_INVDATALEN);
    }
    // read packet identifier
    m_packet_identifier = static_cast<std::uint16_t>(head[7]) |
                          static_cast<std::uint16_t>(head[8]) << 8;

    return {};
}

std::array<unsigned char, header_length>
Packet::header() const noexcept
{
    auto frame_data = m_frame_data_length;
    if (is_response())
    {
        frame_data += response_length;
    }
    // make packet header
    std::array<unsigned char, header_length> ret{
        protocol_version,                                    // 0
        security_key_id,                                     // 1
        flag,                                                // 2
        header_length,                                       // 3
        header_encoding,                                     // 4
        static_cast<std::uint8_t>(frame_data),               // 5
        static_cast<std::uint8_t>(frame_data >> 8),          // 6
        static_cast<std::uint8_t>(m_packet_identifier),      // 7
        static_cast<std::uint8_t>(m_packet_identifier >> 8), // 8
        static_cast<uint8_t>(m_packet_type),                 // 9
    };
    ret[header_length - 1] = egts::v1::crc8(ret.begin(), ret.end() - crc_header_length); // 10
    return ret;
}

frame_buffer_type
Packet::buffer() const noexcept
{
    frame_buffer_type ret(header_length);
    auto h = header();
    std::copy(h.begin(), h.end(), ret.begin());
    if (m_frame_data_length > 0)
    {
        ret.resize(header_length + m_frame_data_length + crc_data_length);
        auto f = frame();
        std::copy(f.begin(), f.end(), ret.begin());
    }
    return std::move(ret);
}
} // namespace egts::v1::transport