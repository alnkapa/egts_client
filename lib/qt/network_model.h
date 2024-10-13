#pragma once
#ifndef NETWORK_MODEL_H
#define NETWORK_MODEL_H

#include "i_network_model.h"
#include <QString>
#include <QTcpSocket>

namespace model
{

class Network : public INetwork, public QObject
{
    Q_OBJECT

  public:
    Network(const QString &host, quint16 port, QObject *parent = nullptr);

    virtual ~Network();
  
    std::size_t
    send(std::uint8_t *ptr, std::size_t size) override;

    std::vector<std::uint8_t>
    receive(std::size_t size) override;

  private:
    QTcpSocket socket;
};

} // namespace model
#endif
