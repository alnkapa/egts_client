#include "network_model.h"


namespace model
{

Network::Network(std::string_view host, std::string_view port)
    : socket(io_context)
{
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::connect(socket, resolver.resolve(host, port));
}

Network::~Network()
{
    if (socket.is_open())
    {
        socket.close();
    }
}

std::size_t
Network::send(std::uint8_t *ptr, std::size_t size)
{
    return boost::asio::write(
        socket,
        boost::asio::buffer({reinterpret_cast<const char *>(ptr), size}),
        boost::asio::transfer_all());
}

std::vector<std::uint8_t>
Network::receive(std::size_t size)
{
    std::vector<std::uint8_t> buffer(size);
    std::size_t n = boost::asio::read(
        socket,
        boost::asio::buffer(buffer),
        boost::asio::transfer_all());
    buffer.resize(n);
    return buffer;
}

} // namespace model
