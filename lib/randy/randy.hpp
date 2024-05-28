#pragma once
#ifndef RANDY_HPP
#define RANDY_HPP

#include <cstdint>
#include <array>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random> // random_device,default_random_engine

namespace randy
{

    template <std::size_t N>
    using bytes_t = std::array<std::uint8_t, N>;

    template <std::size_t N>
    constexpr bytes_t<N> random()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::uint8_t> distribution(0, UINT8_MAX);
        bytes_t<N> ret;
        for (int i = 0; i < N; i++)
        {
            ret[i] = distribution(gen);
        }
        return ret;
    }
    template <std::size_t N>
    std::string to_hex(const bytes_t<N> &buf)
    {
        std::stringstream stream;
        stream.flags(std::ios::hex | std::ios::uppercase);
        for (auto &&b : buf)
        {
            if (b)
            {
                stream << std::setw(2) << std::setfill('0') << static_cast<int>(b);
            }
        };
        return stream.str();
    }
}

#endif /* RANDY_HPP */
