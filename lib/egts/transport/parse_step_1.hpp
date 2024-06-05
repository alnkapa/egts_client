#pragma once
#ifndef PARSE_STEP_1_HPP
#define PARSE_STEP_1_HPP
#include "all.hpp"
#include <variant>

namespace egts::v1::transport
{
    using namespace egts::v1::transport;
    using namespace egts::v1::error;
    Error Packet::parseStep1(uint8_t buf[4]) noexcept
    {
        // версия PRV
        if (buf[0] != version)
        {
            return Error{Error::Code::EGTS_PC_UNS_PROTOCOL};
        }
        // Версия PRF
        std::variant<uint8_t, Packet::Flag> v{buf[2]};
        m_flag = std::get<1>(v);
        if (m_flag.prefix != 0)
        {
            return Error{Error::Code::EGTS_PC_UNS_PROTOCOL};
        }
        // HL==11 или HL==16
        const uint8_t hl = buf[3];
        if (!(hl == 11 && !m_flag.route) && !(hl == 16 && m_flag.route))
        {
            return Error{Error::Code::EGTS_PC_INC_HEADERFORM};
        }       
        return Error{};
    };
}
#endif /* PARSE_STEP_1_HPP */