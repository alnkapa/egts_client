#ifndef TRANSPORT_H
#define TRANSPORT_H
#include <array>
#include <crc/crc.h>
#include <cstdint> // uint8_t
#include <error/error.h>
#include <globals.h>
#include <initializer_list>
#include <stddef.h> // size_t
#include <tuple>

namespace egts::v1::transport
{

using frame_buffer_type = egts::v1::frame_buffer_type;

using record_payload_type = egts::v1::record_payload_type;

using Error = egts::v1::error::Error;

using Code = egts::v1::error::Code;

// length crc of frame data
const uint8_t crc_data_length = 2;

// length crc of header data
const uint8_t crc_header_length = 1;

// length response
const uint8_t response_length = 3;

// protocol version
const std::uint8_t protocol_version{0x1};
// the length of the Transport Layer header in bytes, including the checksum
// byte
const std::uint8_t header_length{11};

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
    frame_buffer_type mp_data;

    // packet type
    Type m_packet_type{Type::EGTS_PT_APPDATA};

    // packet number
    uint16_t m_packet_identifier{0};

    // frame data length
    uint16_t m_frame_data_length{0};

    // response packet number
    uint16_t m_response_packet_identifier{0};

    Error m_processing_result{};

  public:
    void
    identifier(uint16_t packet_identifier);

    uint16_t
    identifier() const;

    bool
    is_response() const;

    std::pair<uint16_t, Error>
    response() const;

    uint16_t
    frame_data_length() const;

    Packet
    make_response(const Error &processing_result);

    Error
    parse_frame(frame_buffer_type &&buffer) noexcept;

    void
    set_frame(frame_buffer_type &&buffer) noexcept;

    record_payload_type
    get_frame() const noexcept;

    frame_buffer_type
    frame_to_buffer() const noexcept;

    Error
    parse_header(const header_buffer_type &) noexcept;

    header_buffer_type
    header_to_buffer() const noexcept;

    frame_buffer_type
    buffer() const noexcept;

    bool
    operator==(const Packet &other) const;
};

} // namespace egts::v1::transport

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
