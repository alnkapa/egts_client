#pragma once
#ifndef NETWORK_MODEL_H
#define NETWORK_MODEL_H

#include <cstdint>
#include <span>
#include <vector>

class INetworkModel
{
  public:
    virtual ~INetworkModel() = default;

    virtual void
    send(std::span<std::uint8_t> view) = 0;

    virtual std::vector<std::uint8_t>
    receive(std::size_t size) = 0;
};

#endif
