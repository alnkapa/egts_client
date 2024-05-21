#pragma once
#ifndef RANDY_HPP
#define RANDY_HPP

#include <cstdint> //uint8_t
#include <array>   //array
#include <random>  // random_device,default_random_engine
#include <iomanip> // std::setw, std::setfill

namespace randy
{
    template <std::size_t N>
    struct bytes
    {
        std::array<std::uint8_t, N> data{};
    };
    template <std::size_t N>
    bytes<N> get_bytes()
    {
        bytes<N> ret{0};
        return ret;
    };
    template <std::size_t N>
    std::ostream &operator<<(std::ostream &out, const bytes<N> &buf)
    {
        out << "ok";
        return out;
    };

    // Как сделать, что бы вот это собиралось?
    template <std::size_t N>
    using bytes_t = std::array<std::uint8_t, N>;

    template <std::size_t N>
    bytes_t<N> get_bytesN()
    {
        bytes_t<N> ret{0};
        return ret;
    };
    template <std::size_t N>
    std::ostream &operator<<(std::ostream &out, const bytes_t<N> &buf)
    {
        out << "ok1";
        return out;
    };
}

#endif /* RANDY_HPP */
