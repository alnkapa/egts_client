#pragma once
#ifndef PACKET_TYPE_HPP
#define PACKET_TYPE_HPP

#include <string_view>

namespace egts
{
    namespace transport
    {
        enum class PacketType : unsigned char
        {
            EGTS_PT_RESPONSE = 0,
            EGTS_PT_APPDATA = 1,
            EGTS_PT_SIGNED_APPDATA = 2,
            low = 0b11,
        };
        std::string_view getPacketType(PacketType p)
        {
            switch (p)
            {
            case PacketType::EGTS_PT_RESPONSE:
                return "EGTS_PT_RESPONSE";
            case PacketType::EGTS_PT_APPDATA:
                return "EGTS_PT_APPDATA";
            case PacketType::EGTS_PT_SIGNED_APPDATA:
            return "EGTS_PT_SIGNED_APPDATA";
            }
            return "???";
        };
    }
}
#endif /* PACKET_TYPE_HPP */