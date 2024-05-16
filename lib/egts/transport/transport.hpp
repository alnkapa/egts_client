#pragma once
#ifndef TRANSPORT_HPP
#define TRANSPORT_HPP

#include "priority.hpp"
#include "packet_type.hpp"
#include <iostream>
#include "../../endian/endian.hpp"

namespace egts
{
    namespace transport
    {
        struct Route
        {
            unsigned short peer_address{};
            unsigned short recipient_address{};
            unsigned short time_to_live{};
        };
        struct Flag
        {
            Priority pr : 2;                   /** Encryption algorithm code for SFRD */
            bool compressed : 1;               /** SFRD is compressed */
            unsigned encryption_algorithm : 2; /** Encryption algorithm code for SFRD */
            bool route : 1;                    /** Packet routing flag */
            unsigned prefix : 2;               /** Transport Layer header prefix */
        };
        /**
         * Composition of the Transport Layer Protocol Packet
         */
        struct Packet
        {
            unsigned char protocol_version{0x01}; /** Protocol version */
            unsigned char security_key_id{0};     /** ID of the key used for encryption */
            union
            {
                Flag flags;
                unsigned char raw_flags{0};
            };
            unsigned char header_length{0}; /** Length including the checksum */
            unsigned char header_encoding{0};
            unsigned short frame_data_length{0};
            unsigned short packet_identifier{0}; /** Package number */
            PacketType packet_type;
            Route route{};
            unsigned short header_check_sum{};
        };
        /**
         * Read Packet
         */
        std::istream &operator>>(std::istream &in, Packet &d) {            
            return in;
        };
    }
}
#endif /* TRANSPORT_HPP */