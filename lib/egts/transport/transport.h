#ifndef TRANSPORT_H
#define TRANSPORT_H
#include <array>
#include <crc/crc.h>
#include <cstdint> // uint8_t, uint16_t
#include <error/error.h>
#include <globals.h>
#include <stddef.h> // size_t

namespace egts::v1::transport
{

using buffer_type = egts::v1::buffer_type;

using payload_type = egts::v1::payload_type;

using Error = egts::v1::error::Error;

using Code = egts::v1::error::Code;

using uint8_t = std::uint8_t;

using uint16_t = std::uint16_t;

// length crc of frame data
const uint8_t crc_data_length = 2;

// length crc of header data
const uint8_t crc_header_length = 1;

// length response
const uint8_t response_length = 3;

// protocol version
const uint8_t protocol_version{0x1};
// the length of the Transport Layer header in bytes, including the checksum
// byte
const uint8_t header_length{11};

using header_buffer_type = std::array<unsigned char, header_length>;

// the parameter SKID defines the identifier of the key used for encryption
constexpr uint8_t security_key_id{0x0};

// transport Layer header flag
constexpr uint8_t flag{0};

// determines the encoding method applied to the part of the Transport Layer
// header following this parameter
constexpr uint8_t header_encoding{0};

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
    // packet payload
    buffer_type mp_data;

    // packet type
    Type m_packet_type{Type::EGTS_PT_APPDATA};

    // packet number
    uint16_t m_packet_identifier{0};

    // frame data length
    uint16_t m_frame_data_length{0};

    // response packet number
    uint16_t m_response_packet_identifier{0};

    Code m_processing_result{};

  public:
    Packet();

    Packet(buffer_type &&frame);

    Packet(uint16_t packet_identifier, buffer_type &&frame);

    void
    identifier(uint16_t packet_identifier);

    uint16_t
    identifier() const;

    bool
    is_response() const;

    std::pair<uint16_t, Code>
    response() const;

    uint16_t
    frame_data_length() const;

    Packet
    make_response(const Code &processing_result);

    void
    parse_frame(buffer_type &&buffer);

    void
    set_frame(buffer_type &&buffer) noexcept;

    payload_type
    get_frame() const noexcept;

    buffer_type
    frame_to_buffer() const;

    void
    parse_header(const header_buffer_type &);

    header_buffer_type
    header_to_buffer() const noexcept;

    buffer_type
    buffer() const noexcept;

    bool
    operator==(const Packet &other) const;
};

} // namespace egts::v1::transport

std::ostream &
operator<<(std::ostream &os, const egts::v1::transport::Type &type);

namespace std
{
template <>
struct hash<egts::v1::transport::Packet>
{
    std::size_t
    operator()(const egts::v1::transport::Packet &in) const
    {
        return static_cast<std::size_t>(in.identifier());
    }
};
} // namespace std

#endif /* TRANSPORT_H */
