#ifndef NETWORK_MODEL_H
#define NETWORK_MODEL_H

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <network_model.h>

class NetworkModel: public INetworkModel
{
  public:
    NetworkModel(const std::string &host, const std::string &port);
    void
    sendMessage(const std::string &message);
    std::string
    receiveMessage();

  private:
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket;
};

#endif
