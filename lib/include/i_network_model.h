#pragma once
#ifndef I_NETWORK_MODEL_H
#define I_NETWORK_MODEL_H

#include <cstdint>
#include <vector>

namespace model
{

class INetwork
{
  public:
    virtual ~INetwork() = default;

    virtual std::size_t
    send(std::uint8_t *ptr, std::size_t size) = 0;

    virtual std::vector<std::uint8_t>
    receive(std::size_t size) = 0;
};

} // namespace model
#endif
