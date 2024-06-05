#pragma once
#ifndef TRANSPORT_HPP
#define TRANSPORT_HPP

#include "error.hpp"
#include <iostream>
#include <cstdint>  // uint8_t
#include <stddef.h> // size_t
#include <array>

namespace egts::v1::transport
{
    using namespace std;
    using namespace error;
    static const uint8_t version = {0x1}; // protocol version

    class Packet
    {
    public:
        // packet type
        enum class Type : uint8_t
        {
            EGTS_PT_RESPONSE = 0,       // package confirmation
            EGTS_PT_APPDATA = 1,        // new package
            EGTS_PT_SIGNED_APPDATA = 2, // new package with encryption
        };
        /**
         * routing priority
         */
        enum class Priority : uint8_t
        {
            highest = 0b00,
            high = 0b01,
            middle = 0b10,
            low = 0b11,
        };
        // packet flag
        struct Flag
        {
            Priority pr : 2;              /** routing priority */
            bool compressed : 1;          /** SFRD is compressed */
            int encryption_algorithm : 2; /** Encryption algorithm code for SFRD */
            bool route : 1;               /** Packet routing flag */
            int prefix : 2;               /** Transport Layer header prefix */
        };
        /*! шаг 1
         *
         * Проверка PRV, PRF, HL
         */
        Error parseStep1(array<uint8_t, 4>) noexcept;
        /*! шаг 2
         *
         * чтение оставшихся 6 или 11 байт и проверка CRC8
         */
        template <size_t N>            
        Error parseStep2(array<uint8_t, N>) noexcept;
        Type getType()
        {
            return m_packet_type;
        };

    private:
        Type m_packet_type{Packet::Type::EGTS_PT_APPDATA};
        Flag m_flag{};
        uint8_t crc{};
        // std::uint8_t protocol_version{0x01}; /** Protocol version */
        // std::uint8_t security_key_id{0};     /** ID of the key used for encryption */

        //  std::uint8_t header_length{0}; /** Length including the checksum */
        //  std::uint8_t header_encoding{0};
        //  std::uint16_t frame_data_length{0};
        //  std::uint16_t packet_identifier{0}; /** Package number */

        // std::optional<Route> route;
        // std::uint8_t header_check_sum{};
    };

    // template <typename Stream>
    // void read(Stream &stream, Packet &packet)
    // {
    //     std::cout << "len:" << offsetof(Packet, header_encoding) << "\n";
    //     stream.read(reinterpret_cast<char *>(&packet.protocol_version), offsetof(Packet, header_encoding));
    //     // stream.read(reinterpret_cast<char *>(&packet.frame_data_length), sizeof(packet.frame_data_length));
    //     // // packet.frame_data_length = endian::reverse(packet.frame_data_length);
    //     // stream.read(reinterpret_cast<char *>(&packet.packet_identifier), sizeof(packet.packet_identifier));
    //     // // packet.packet_identifier = endian::reverse(packet.packet_identifier);
    //     // stream.read(reinterpret_cast<char *>(&packet.packet_type), sizeof(packet.packet_type));
    //     // if (packet.flags.route)
    //     // {
    //     //     Route r{};
    //     //     stream.read(reinterpret_cast<char *>(&r.peer_address), offsetof(Route, time_to_live));
    //     //     // r.peer_address = endian::reverse(r.peer_address);
    //     //     // r.recipient_address = endian::reverse(r.recipient_address);
    //     //     // r.time_to_live = endian::reverse(r.time_to_live);
    //     //     packet.route = r;
    //     // }
    //     // stream.read(reinterpret_cast<char *>(&packet.header_check_sum), sizeof(packet.header_check_sum));
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
    //     out << "encryption_algorithm:" << std::hex << packet.flags.encryption_algorithm << "\n";
    //     out << "compressed:" << std::hex << packet.flags.compressed << "\n";
    //     egts::transport::getPriority(packet.flags.pr);
    //     out << "Priority:" << egts::transport::getPriority(packet.flags.pr) << "\n";
    //     out << "\n";
    //     return out;
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
    // /**
    //  * under construction
    //  */
    // struct Route
    // {
    //     std::uint16_t peer_address{};      // under construction
    //     std::uint16_t recipient_address{}; // under construction
    //     std::uint16_t time_to_live{};      // under construction
    // };
    // struct Flag
    // {
    //     Priority pr : 2;              /** Encryption algorithm code for SFRD */
    //     bool compressed : 1;          /** SFRD is compressed */
    //     int encryption_algorithm : 2; /** Encryption algorithm code for SFRD */
    //     bool route : 1;               /** Packet routing flag */
    //     int prefix : 2;               /** Transport Layer header prefix */
    // };
}
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
// #endif /* PRIORITY_HPP */