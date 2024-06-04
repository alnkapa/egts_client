#pragma once
#ifndef RANDY_HPP
#define RANDY_HPP

#include <algorithm>
#include <array>
#include <random> // random_device,default_random_engine

namespace randy
{
    template <typename T, std::size_t N>
    class Array : public std::array<T, N>
    {
    public:
        Array() : std::array<T, N>()
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<T> distribution(0, UINT8_MAX);
            for (int i = 0; i < N; i++)
            {
                (*this)[i] = distribution(gen);
            }
        };
    };
}

#endif /* RANDY_HPP */
