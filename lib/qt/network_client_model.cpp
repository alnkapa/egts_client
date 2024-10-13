#include "network_client_model.h"
#include <cstddef>

namespace model
{

NetworkClient::NetworkClient(const QString &host, quint16 port, QObject *parent)
    : QObject(parent)
{
    socket.connectToHost(host, port);
}

NetworkClient::~NetworkClient()
{
    if (socket.isOpen())
    {
        socket.close();
    }
}

std::size_t
NetworkClient::send(std::uint8_t *ptr, std::size_t size)
{
    std::size_t total = 0;
    QByteArray data(reinterpret_cast<const char *>(ptr), size);
    while (total < size)
    {
        total += socket.write(data.mid(total));
        socket.flush();
    }
    return total;
}

std::vector<std::uint8_t>
NetworkClient::receive(std::size_t size)
{
    std::vector<std::uint8_t> buffer(size);
    std::size_t total = 0;
    while (total < size)
    {
        std::size_t n = socket.read(reinterpret_cast<char *>(buffer.data() + total), size - total);
        if (n == 0)
        {
            break;
        }
        total += n;
    }
    buffer.resize(total);
    return buffer;
}

} // namespace model
