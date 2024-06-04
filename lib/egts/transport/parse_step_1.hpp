#pragma once
#ifndef PARSE_STEP_1_HPP
#define PARSE_STEP_1_HPP
#include "all.hpp"

namespace egts::v1::transport
{
    Error Packet::parseStep1(uint8_t buf[3]) noexcept
    {
        // uint8_t buf[16]{};
        // try
        // {
        //     int step{};
        //     bool isNext{true};
        //     while (isNext)
        //     {
        //         switch (step)
        //         {
        //         case 0: // Версия PRV И PRF поддерживается?
        //             stream.read(reinterpret_cast<char *>(&buf), 3);
        //             if (buf[0] != version) {

        //             }
        //             break;
        //         default:
        //             isNext = false;
        //             break;
        //         }
        //     }
        //     std::cout << "" ;
        // }
        // catch (...)
        // {
        //     std::cout << "Error" << "\n";
        // }
        return Error{};
    };
}
#endif /* PARSE_STEP_1_HPP */