#pragma once
#ifndef TRANSPORT_HPP
#define TRANSPORT_HPP
#include <stddef.h>  // size_t
#include <cstdint>   // uint8_t
#include <memory>
#include <utility>       // move
#include "../crc/crc.h"  // uint8_t
#include "../egts.h"     // uint8_t
#include "error.hpp"

namespace egts::v1::transport {
using namespace std;
using namespace error;

// protocol version
constexpr uint8_t protocol_version = {0x1};

// the parameter SKID defines the identifier of the key used for encryption
constexpr uint8_t security_key_id = {0x0};

// transport Layer header prefix
constexpr uint8_t prefix = {(0x0 << 6) & 0xC0};

// the length of the Transport Layer header in bytes, including the checksum
// byte
constexpr uint8_t header_length = {11};

// determines the encoding method applied to the part of the Transport Layer
// header following this parameter
constexpr uint8_t header_encoding = {0};

// packet type
enum class Type : uint8_t {
    EGTS_PT_RESPONSE = 0,        // package confirmation
    EGTS_PT_APPDATA = 1,         // new package
    EGTS_PT_SIGNED_APPDATA = 2,  // new package with encryption
};

class Packet : public egts::v1::Payload {
   private:

    // generates values for fields PRF RTE ENA CMP PR
    uint8_t flag() {
        return prefix;
    };
    // packet payload
    weak_ptr<egts::v1::Payload> mp_data;
    // packet type
    Type m_packet_type{Type::EGTS_PT_APPDATA};
    // package number
    uint16_t m_packet_identifier{0};
    // response package number
    uint16_t m_response_packet_identifier{0};
    Error m_processing_result{};
    // std::uint8_t header_check_sum{};
    //  uint8_t crc{};
   public:

    Packet(uint16_t identifier = 0) : m_packet_identifier(identifier){};

    void response(uint16_t response_packet_identifier,
                  egts::v1::error::Error processing_result) {
        m_packet_type = Type::EGTS_PT_RESPONSE;
        m_response_packet_identifier = response_packet_identifier;
        m_processing_result = processing_result;
    };

    void set_data(weak_ptr<egts::v1::Payload> data) {
        mp_data = move(data);
    };

    // pack packet to Buffer
    egts::v1::Buffer pack() {
        // FrameData
        egts::v1::Buffer frame_data{};
        egts::v1::Buffer response_buf{};
        uint16_t crc16_val = 0;
        if (m_packet_type == Type::EGTS_PT_RESPONSE) {  // prepend response id
            egts::v1::Buffer response_buf{
                m_response_packet_identifier,  // response_packet_identifier (2
                                               // byte)
                static_cast<uint8_t>(
                    m_processing_result)  // processing_result (1 byte)
            };
            crc16_val =
                egts::v1::crc16(response_buf.begin(), response_buf.end());
        }
        if (auto ptr = mp_data.lock()) {  // packet has payload
            frame_data = std::move(ptr->pack());
            crc16_val = egts::v1::crc16(frame_data.begin(), frame_data.end(),
                                        crc16_val);
        }
        uint32_t frame_size = response_buf.size() + frame_data.size();
        if (frame_size > std::numeric_limits<uint16_t>::max()) {
            throw std::overflow_error("Frame size exceeds maximum limit");
        }
        // make packet header
        egts::v1::Buffer rez{
            protocol_version,                     // protocol version  (byte)
            security_key_id,                      // SKID (byte)
            flag(),                               // flag (byte)
            header_length,                        // header_length (byte)
            header_encoding,                      // header_encoding (byte)
            static_cast<uint16_t>(frame_size),    // frame data size (2 byte)
            m_packet_identifier,                  // packet_identifier (2 byte)
            static_cast<uint8_t>(m_packet_type),  // packet_type (byte)
        };
        // header check sum
        rez.push_back(egts::v1::crc8(rez.begin(), rez.end()));

        if (frame_size > 0) {
            // response packet type
            if (m_packet_type == Type::EGTS_PT_RESPONSE) {
                rez.push_back(response_buf);
            }
            // services frame data
            if (!frame_data.empty()) {
                rez.push_back(frame_data);
            }
            rez.push_back(crc16_val);
        }

        return std::move(rez);
    };
};

// template <typename Stream>
// void read(Stream &stream, Packet &packet)
// {
//     std::cout << "len:" << offsetof(Packet, header_encoding) << "\n";
//     stream.read(reinterpret_cast<char *>(&packet.protocol_version),
//     offsetof(Packet, header_encoding));
//     // stream.read(reinterpret_cast<char *>(&packet.frame_data_length),
//     sizeof(packet.frame_data_length));
//     // // packet.frame_data_length =
//     endian::reverse(packet.frame_data_length);
//     // stream.read(reinterpret_cast<char *>(&packet.packet_identifier),
//     sizeof(packet.packet_identifier));
//     // // packet.packet_identifier =
//     endian::reverse(packet.packet_identifier);
//     // stream.read(reinterpret_cast<char *>(&packet.packet_type),
//     sizeof(packet.packet_type));
//     // if (packet.flags.route)
//     // {
//     //     Route r{};
//     //     stream.read(reinterpret_cast<char *>(&r.peer_address),
//     offsetof(Route, time_to_live));
//     //     // r.peer_address = endian::reverse(r.peer_address);
//     //     // r.recipient_address = endian::reverse(r.recipient_address);
//     //     // r.time_to_live = endian::reverse(r.time_to_live);
//     //     packet.route = r;
//     // }
//     // stream.read(reinterpret_cast<char *>(&packet.header_check_sum),
//     sizeof(packet.header_check_sum));
// }

// struct Flag
// {
//     Priority pr : 2;              /** Encryption algorithm code for SFRD */
//     bool compressed : 1;          /** SFRD is compressed */
//     int encryption_algorithm : 2; /** Encryption algorithm code for SFRD */
//     bool route : 1;               /** Packet routing flag */
//     int prefix : 2;               /** Transport Layer header prefix */
// };
}  // namespace egts::v1::transport
#endif /* TRANSPORT_HPP */
