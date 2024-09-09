#include "lib/egts/error/error.h"
#include "lib/egts/transport/transport.h"
#include <boost/asio.hpp>
#include <iostream>

class ReaderWriter : public std::enable_shared_from_this<ReaderWriter>
{
  private:
    std::array<std::uint8_t, egts::v1::transport::header_length> m_header;
    std::vector<std::uint8_t> m_frame;
    boost::asio::ip::tcp::socket m_socket;
    std::shared_ptr<egts::v1::transport::Packet> m_pkg;

    void
    do_read_header()
    {
        boost::asio::async_read(
            m_socket,
            boost::asio::buffer(m_header),
            [self = shared_from_this()](
                const boost::system::error_code &ec,
                std::size_t bytes_transferred)
            {
                if (!ec)
                {
                    auto err = self->m_pkg->parse_header(self->m_header);
                    if (err == egts::v1::error::Code::EGTS_PC_OK)
                    {
                        if (self->m_pkg->frame_data_length() > 0)
                        {
                            self->do_read_frame(
                                self->m_pkg->frame_data_length() +
                                egts::v1::transport::crc_data_length);
                        }
                        else
                        {
                            // TODO: response for packet is received
                            self->do_read_header();
                        }
                    }
                    else
                    {
                        // TODO: response error and disconnect
                    }
                }
                else
                {
                    std::cerr << "Error reading header: " << ec.message() << std::endl;
                }
            });
    }

    void
    do_read_frame(size_t frame_size)
    {
        m_frame.reserve(frame_size);
        boost::asio::async_read(
            m_socket,
            boost::asio::buffer(m_frame),
            [self = shared_from_this(), frame_size](
                const boost::system::error_code &ec,
                std::size_t bytes_transferred)
            {
                if (!ec)
                {
                    auto err = self->m_pkg->parse_frame(std::move(self->m_frame));
                    if (err == egts::v1::error::Code::EGTS_PC_OK)
                    {
                        // TODO: response for packet is received
                        self->do_read_header();
                    }
                    else
                    {
                        // TODO: response error and disconnect
                    }
                }
                else
                {
                    std::cerr << "Error reading frame: " << ec.message() << std::endl;
                }
            });
    }

  public:
    ReaderWriter(boost::asio::ip::tcp::socket socket)
        : m_socket(std::move(socket)),
          m_pkg(std::make_shared<egts::v1::transport::Packet>()) {}

    void
    start()
    {
        // подписаться на очередь для получения точек

        // запустить процесс чтение ответов от сервера
        do_read_header();
    }
};

int
main(int argc, char *argv[])
{
    boost::asio::io_context io_context{};
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints;
    boost::asio::ip::tcp::socket socket(io_context);
    try
    {
        endpoints = resolver.resolve("alfa.shatl-t.ru", "34329");
        boost::asio::connect(socket, endpoints);
        std::make_shared<ReaderWriter>(std::move(socket))->start();
        io_context.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    };
    return 0;
}
