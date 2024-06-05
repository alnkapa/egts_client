#pragma once
#ifndef PARSE_STEP_1_HPP
#define PARSE_STEP_1_HPP
#include "all.hpp"
#include <variant>

namespace egts::v1::transport
{
    using namespace egts::v1::transport;
    using namespace egts::v1::error;
    /*! Чтение заголовка, шаг 1
     *
     * Проверка PRV, PRF, HL
     */
    Error Packet::parseStep1(array<uint8_t, 4> buf) noexcept
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
        // TODO: calc crc8
        return Error{};
    };
    /*! Чтение заголовка, шаг 2
     *
     * чтение оставшихся 6 или 11 байт и проверка CRC8
     */
    template <size_t N>
    Error parseStep2(array<uint8_t, N>) noexcept
    {        
        // TODO: calc crc8        
        return Error{};
    }    
};
#endif /* PARSE_STEP_1_HPP */