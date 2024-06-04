#pragma once
#ifndef RANDY_HPP
#define RANDY_HPP
#include <cstddef> // size_t
#include <array>
#include <concepts> // std::integral
#include <random>   // random_device,default_random_engine

namespace randy
{
    using namespace std;
    template <typename T, size_t N>
        requires(integral<T>)
    class Array : public array<T, N>
    {
    public:
        void generate()
        {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<T> distribution(0, UINT8_MAX);
            for (int i = 0; i < N; i++)
            {
                (*this)[i] = distribution(gen);
            }
        };
        Array() : array<T, N>{}
        {
            generate();
        };
    };
}

#endif /* RANDY_HPP */
