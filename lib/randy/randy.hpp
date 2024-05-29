#pragma once
#ifndef RANDY_HPP
#define RANDY_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random> // random_device,default_random_engine

namespace randy
{
    class Bytes
    {
    public:
        using bytes_t = std::vector<std::uint8_t>;
        Bytes() = default;
        Bytes(const bytes_t &data) : m_data(data) {}
        Bytes(bytes_t &&data) : m_data(std::move(data)) {}

        const bytes_t &data() const { return m_data; }
        bytes_t &data() { return m_data; }

        uint8_t &operator[](std::size_t index) { return m_data[index]; }
        const uint8_t &operator[](std::size_t index) const { return m_data[index]; }

        std::string to_hex_string()
        {
            std::stringstream stream;
            stream.flags(std::ios::hex | std::ios::uppercase);
            for (auto &&b : m_data)
            {
                if (b)
                {
                    stream << std::setw(2) << std::setfill('0') << static_cast<int>(b);
                }
            };
            return stream.str();
        }

    private:
        bytes_t m_data;
    };
    template <std::size_t N>
    constexpr Bytes random()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::uint8_t> distribution(0, UINT8_MAX);
        Bytes::bytes_t ret{};
        ret.reserve(N);
        for (int i = 0; i < N; i++)
        {
            ret[i] = distribution(gen);
        }
        return ret;
    }
}

#endif /* RANDY_HPP */
