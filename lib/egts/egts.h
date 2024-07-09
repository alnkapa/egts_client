#pragma once
#ifndef EGTS_H
#define EGTS_H

#include <stddef.h> // size_t
#include <cstdint>  // uint8_t
#include <vector>
#include <memory>

namespace egts::v1
{
    class Payload
    {
        virtual std::vector<std::uint8_t> pack() = 0;
        virtual void set_data(std::weak_ptr<Payload>) = 0;
    };

}

#endif /* EGTS_H */
