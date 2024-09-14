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

bool
Packet::is_response() const
{
    return m_packet_type == Type::EGTS_PT_RESPONSE;
}

uint16_t
Packet::frame_data_length() const
{
    return m_frame_data_length;
}

using Error = egts::v1::error::Error;
using Code = egts::v1::error::Code;

Error
Packet::parse_frame(std::vector<unsigned char> &&buffer) noexcept
{
    // test size of frame size
    if (buffer.size() != m_frame_data_length + crc_data_length)
    {
        return Error(Code::EGTS_PC_INVDATALEN);
    }
    mp_data = std::move(buffer);
    // test crc
    uint16_t crc16_val = egts::v1::crc16(mp_data.begin(), mp_data.end() - crc_data_length);

    uint16_t crc_begin_index = m_frame_data_length;
    uint16_t got_crc16_val = static_cast<std::uint16_t>(mp_data[crc_begin_index]) |
                             static_cast<std::uint16_t>(mp_data[crc_begin_index + 1]) << 8;

    if (crc16_val != got_crc16_val)
    {
        return Error(Code::EGTS_PC_DATACRC_ERROR);
    }
    // cut crc value
    mp_data.resize(m_frame_data_length);
    return {};
}

std::vector<unsigned char>
Packet::frame() const noexcept
{
    assert(m_frame_data_length != mp_data.size());
    std::vector<unsigned char> ret(m_frame_data_length + crc_data_length);
    std::copy(mp_data.begin(), mp_data.end(), ret.begin());
    uint16_t crc16_val = egts::v1::crc16(mp_data.begin(), mp_data.end());
    ret[m_frame_data_length] = static_cast<std::uint8_t>(crc16_val);
    ret[m_frame_data_length + 1] = static_cast<std::uint8_t>(crc16_val >> 8);
    return std::move(ret);
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
    // read packet identifier
    m_packet_identifier = static_cast<std::uint16_t>(head[7]) |
                          static_cast<std::uint16_t>(head[8]) << 8;

    return {};
}

std::array<unsigned char, header_length>
Packet::header() const noexcept
{
    // make packet header
    std::array<unsigned char, header_length> ret{
        protocol_version,                                    // 0
        security_key_id,                                     // 1
        flag,                                                // 2
        header_length,                                       // 3
        header_encoding,                                     // 4
        static_cast<std::uint8_t>(m_frame_data_length),      // 5
        static_cast<std::uint8_t>(m_frame_data_length >> 8), // 6
        static_cast<std::uint8_t>(m_packet_identifier),      // 7
        static_cast<std::uint8_t>(m_packet_identifier >> 8), // 8
        static_cast<uint8_t>(m_packet_type),                 // 9
    };
    ret[header_length - 1] = egts::v1::crc8(ret.begin(), ret.end() - crc_header_length); // 10
    return ret;
}

std::vector<unsigned char>
Packet::buffer() const noexcept
{
    std::vector<unsigned char> ret(header_length);
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