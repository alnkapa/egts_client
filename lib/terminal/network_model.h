#pragma once
#ifndef NETWORK_MODEL_H
#define NETWORK_MODEL_H

#include "i_network_model.h"
#include <boost/asio.hpp>
#include <string_view>

namespace model
{

class Network : public INetwork
{
  public:
    Network(std::string_view host, std::string_view port);

    virtual ~Network();

    std::size_t
    send(std::uint8_t *ptr, std::size_t size) override;

    std::vector<std::uint8_t>
    receive(std::size_t size) override;

  private:
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket;
};

} // namespace model
#endif
