#pragma once
#ifndef TRANSPORT_HPP
#define TRANSPORT_HPP
#include "../crc/crc.h"
#include "../egts.h"
#include "../error/error.h"
#include <array>
#include <cstdint> // uint8_t
#include <memory>
#include <stddef.h> // size_t
#include <vector>

namespace egts::v1::transport
{

// length crc of frame data
const u_int8_t crc_data_length = 2;

// length crc of header data
const u_int8_t crc_header_length = 1;

// protocol version
const std::uint8_t protocol_version{0x1};
// the length of the Transport Layer header in bytes, including the checksum
// byte
const std::uint8_t header_length{11};

// // the parameter SKID defines the identifier of the key used for encryption
// constexpr uint8_t security_key_id{0x0};

// // transport Layer header prefix
// constexpr uint8_t prefix{(0x0 << 6) & 0xC0};

// // determines the encoding method applied to the part of the Transport Layer
// // header following this parameter
// constexpr uint8_t header_encoding{0};

// maximum size of the frame data
constexpr uint16_t max_frame_size{65517};

// packet type
enum class Type : uint8_t
{
    EGTS_PT_RESPONSE = 0,       // packet confirmation
    EGTS_PT_APPDATA = 1,        // new packet
    EGTS_PT_SIGNED_APPDATA = 2, // new packet with encryption
};

class Packet
{
  private:
    // generates values for fields PRF RTE ENA CMP PR
    // uint8_t
    // flag()
    // {
    //     return prefix;
    // };
    // packet payload
    std::vector<unsigned char> mp_data;

    // packet type
    Type m_packet_type{Type::EGTS_PT_APPDATA};

    // packet number
    uint16_t m_packet_identifier{0};

    // frame data length
    uint16_t m_frame_data_length{0};

    // response packet number
    uint16_t m_response_packet_identifier{0};

    error::Error m_processing_result{};

  public:
    Packet(uint16_t identifier = 0);

    virtual ~Packet();

    uint16_t
    packet_identifier() const;

    uint16_t
    frame_data_length() const;

    void
    response(uint16_t response_packet_identifier, egts::v1::error::Error processing_result)
    {
        m_packet_type = Type::EGTS_PT_RESPONSE;
        m_response_packet_identifier = response_packet_identifier;
        m_processing_result = processing_result;
    };

    // parse frame
    error::Error
    parse_frame(std::vector<unsigned char> &&buffer) noexcept;

    // pack header
    std::vector<unsigned char>
    frame() const noexcept;

    // parse header
    error::Error
    parse_header(const std::array<unsigned char, header_length> &) noexcept;

    // pack header
    std::array<unsigned char, header_length>
    header() const noexcept;
};

} // namespace egts::v1::transport
#endif /* TRANSPORT_HPP */