#pragma once
#ifndef RANDY_HPP
#define RANDY_HPP

#include <array>
#include <concepts>
#include <random> // random_device,default_random_engine

namespace randy
{
    template <typename T, std::size_t N>
    requires (std::integral<T>)
    class Array : public std::array<T, N>
    {
    public:
        void generate()
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<T> distribution(0, UINT8_MAX);
            for (int i = 0; i < N; i++)
            {
                (*this)[i] = distribution(gen);
            }
        };
        Array() : std::array<T, N>()
        {
            generate();
        };
    };
}

#endif /* RANDY_HPP */
