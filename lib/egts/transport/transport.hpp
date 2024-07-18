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
    weak_ptr<egts::v1::Payload> mp_data;
    Type m_packet_type{Type::EGTS_PT_APPDATA};
    uint16_t m_packet_identifier{0}; /** Package number */
    uint8_t m_flag;
    // std::uint8_t header_check_sum{};
    //  uint8_t crc{};
   public:

    Packet(uint16_t identifier = 0, Type type = Type::EGTS_PT_APPDATA)
        : m_packet_identifier(identifier),
          m_packet_type(type){};
    void set_data(weak_ptr<egts::v1::Payload> data) {
        mp_data = move(data);
    };
    egts::v1::Buffer pack() {
        egts::v1::Buffer frame_data{};  // FrameData
        if (auto ptr = mp_data.lock()) {
            frame_data = std::move(ptr->pack());
        }
        egts::v1::Buffer rez{
            protocol_version,    security_key_id,        flag(),
            header_length,       header_encoding,        frame_data.size(),
            m_packet_identifier, uint8_t(m_packet_type),
        };
        uint8_t crc{};
        for (const auto& val : rez) {            
            std::cout << static_cast<unsigned long long>(val) << " ";
            crc = crc8(val)
        }
        cout << "\n";
        auto crc = CRC();
        cout << crc(rez.begin(), rez.end()) << "\n";
        cout << "\n";
        cout << crc(rez) << "\n";

        // HeaderCheckSum

        // ServicesFrameData

        // Services Frame Data Check Sum
        return std::move(rez);
    };
};
/*
Прочитать 1 байт
Прочитать 2 байт
---------------
Прочитать 4 байт

ИТОГО прочитать 4 байта.

*/

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

// std::istream &operator>>(std::istream &is, Packet &packet)
// {
//     read(is, packet);
//     return is;
// }
// std::ostream &operator<<(std::ostream &out, const Packet &packet)
// {
//     out << "prefix:" << std::hex << packet.flags.prefix << "\n";
//     out << "route:" << std::hex << packet.flags.route << "\n";
//     out << "encryption_algorithm:" << std::hex <<
//     packet.flags.encryption_algorithm << "\n"; out << "compressed:" <<
//     std::hex << packet.flags.compressed << "\n";
//     egts::transport::getPriority(packet.flags.pr);
//     out << "Priority:" << egts::transport::getPriority(packet.flags.pr) <<
//     "\n"; out << "\n"; return out;
// }
// std::string getPacketType(Type p)
// {
//     switch (p)
//     {
//     case Type::EGTS_PT_RESPONSE:
//         return "EGTS_PT_RESPONSE";
//     case Type::EGTS_PT_APPDATA:
//         return "EGTS_PT_APPDATA";
//     case Type::EGTS_PT_SIGNED_APPDATA:
//         return "EGTS_PT_SIGNED_APPDATA";
//     }
//     return "???";
// };

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

// namespace egts
// {
//     namespace transport
//     {
//         /**
//          * routing priority
//          */
//         enum class Priority : unsigned char
//         {
//             highest = 0b00,
//             high = 0b01,
//             middle = 0b10,
//             low = 0b11,
//         };
//         std::string getPriority(Priority p)
//         {
//             switch (p)
//             {
//             case Priority::highest:
//                 return "highest";
//             case Priority::high:
//                 return "high";
//             case Priority::middle:
//                 return "middle";
//             case Priority::low:
//                 return "low";
//             }
//             return "???";
//         };
//     }
// }