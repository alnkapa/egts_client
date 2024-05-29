#pragma once
#ifndef ENDIAN_HPP
#define ENDIAN_HPP

#include <cstdint> //uint8_t

namespace endian
{
    template <typename T>
    T reverse(T &in)
    {
        const size_t size = sizeof(T);
        using b_array = std::uint8_t[size];
        union
        {
            T data{};
            std::uint8_t byte[size];
        } source, dest;
        source.data = in;
        for (size_t k = 0; k < size; k++)
        {
            dest.byte[k] = source.byte[size - k - 1];
        }
        return dest.data;
    };
}
#endif /* ENDIAN_HPP */
