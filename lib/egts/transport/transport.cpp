#include "transport.h"
#include "crc.h"
#include "error.h"

namespace egts::v1::transport
{

Packet::Packet(uint16_t identifier)
    : m_packet_identifier(identifier) {}

Packet::~Packet() {}

uint16_t
Packet::packet_identifier() const
{
    return m_packet_identifier;
}

uint16_t
Packet::frame_data_length() const
{
    return m_frame_data_length;
}

// void
// Packet::set_data(weak_ptr<egts::v1::Payload> data)
// {
// }

// egts::v1::Buffer
// Packet::pack()
// {
//     egts::v1::Buffer frame_data{};
//     egts::v1::Buffer response_buf{};
//     uint16_t crc16_val = egts::v1::init_crc16;
//     if (m_packet_type == Type::EGTS_PT_RESPONSE)
//     { // prepend response id
//         egts::v1::Buffer response_buf{
//             m_response_packet_identifier,             // 2 byte
//             static_cast<uint8_t>(m_processing_result) // byte
//         };
//         crc16_val = egts::v1::crc16(response_buf.begin(), response_buf.end());
//     }
//     if (auto ptr = mp_data.lock())
//     { // packet has payload
//         frame_data = std::move(ptr->pack());
//         crc16_val = egts::v1::crc16(frame_data.begin(), frame_data.end(), crc16_val);
//     }
//     uint32_t frame_size = response_buf.size() + frame_data.size();
//     if (frame_size > max_frame_size)
//     {
//         throw std::overflow_error("Frame size exceeds maximum limit");
//     }
//     // make packet header
//     egts::v1::Buffer rez{
//         protocol_version,                    // byte
//         security_key_id,                     // byte
//         flag(),                              // byte
//         header_length,                       // byte
//         header_encoding,                     // byte
//         static_cast<uint16_t>(frame_size),   // 2 byte
//         m_packet_identifier,                 // 2 byte
//         static_cast<uint8_t>(m_packet_type), // byte
//     };
//     // header check sum
//     rez.push_back(egts::v1::crc8(rez.begin(), rez.end()));

//     if (frame_size > 0)
//     {
//         // response packet type
//         if (m_packet_type == Type::EGTS_PT_RESPONSE)
//         {
//             rez.push_back(std::move(response_buf));
//         }
//         // services frame data
//         if (!frame_data.empty())
//         {
//             rez.push_back(std::move(frame_data));
//         }
//         rez.push_back(crc16_val);
//     }

//     return std::move(rez);
// };
//     return egts::v1::Buffer();
// }

using Error = egts::v1::error::Error;
using Code = egts::v1::error::Code;

Error
Packet::parse_frame(std::vector<unsigned char> &&buffer) noexcept
{
    mp_data = std::move(buffer);
    // test size of frame size
    if (mp_data.size() != m_frame_data_length + crc_data_length)
    {
        return Error(Code::EGTS_PC_INVDATALEN);
    }
    // test crc
    uint16_t crc16_val = egts::v1::crc16(mp_data.begin(), mp_data.end() - crc_data_length);

    uint16_t crc_begin_index = m_frame_data_length;
    uint16_t got_crc16_val = static_cast<std::uint16_t>(mp_data[crc_begin_index]) |
                             static_cast<std::uint16_t>(mp_data[crc_begin_index + 1]) << 8;

    if (crc16_val != got_crc16_val)
    {
        return Error(Code::EGTS_PC_DATACRC_ERROR);
    }
    return {};
}

std::vector<unsigned char>
Packet::frame() const noexcept
{
    return std::vector<unsigned char>();
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
    return std::array<unsigned char, header_length>();
}
} // namespace egts::v1::transport