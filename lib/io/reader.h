#pragma once
#ifndef READER_H
#define READER_H
#include <cstddef>
#include <cstdint>

namespace io
{
    using size_t = std::size_t;
    using uint8_t = std::uint8_t;

    /**
     * The reader interface
     */
    class Reader
    {
    public:
        virtual Reader read(uint8_t* buf, size_t count) = 0;
    };
}

#endif