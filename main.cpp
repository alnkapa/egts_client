#include "lib/egts/error/error.h"
#include "lib/egts/transport/transport.h"
#include <boost/asio.hpp>
#include <iostream>

class ReaderWriter : public std::enable_shared_from_this<ReaderWriter>
{
  private:
    std::array<unsigned char, egts::v1::transport::header_length> m_header;
    std::vector<unsigned char> m_frame;
    boost::asio::ip::tcp::socket m_socket;
    std::shared_ptr<egts::v1::transport::Packet> m_pkg;

    void
    do_write_error()
    {
        // EXIT
    }

    void
    do_write_header()
    {
        boost::asio::async_write(
            m_socket,
            boost::asio::buffer(m_pkg->header()),
            [self = shared_from_this()](
                const boost::system::error_code &ec,
                std::size_t bytes_transferred)
            {
                if (!ec)
                {

                    if (self->m_pkg->frame_data_length() > 0)
                    {
                        self->do_write_frame();
                    }
                    else
                    {
                        self->do_read_header();
                    }
                }
                else
                {
                    std::cerr << "Error writing header: " << ec.message() << std::endl;
                }
            });
    }

    void
    do_write_frame()
    {
        boost::asio::async_write(
            m_socket,
            boost::asio::buffer(m_pkg->frame()),
            [self = shared_from_this()](
                const boost::system::error_code &ec,
                std::size_t bytes_transferred)
            {
                if (!ec)
                {
                    self->do_read_header();
                }
                else
                {
                    std::cerr << "Error writing frame: " << ec.message() << std::endl;
                }
            });
    }

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
                            self->do_write_header();
                        }
                    }
                    else
                    {
                        self->do_write_error();
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
                        self->do_write_header();
                    }
                    else
                    {
                        self->do_write_error();
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
