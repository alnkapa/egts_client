#include "lib/egts/error/error.h"
#include "lib/egts/transport/transport.h"
#include <boost/asio.hpp>
#include <iostream>

class StateLessReaderWriter : public std::enable_shared_from_this<StateLessReaderWriter>
{
  private:
    boost::asio::ip::tcp::socket m_socket;

    void
    do_write_header()
    {
        auto pkg = std::make_unique<egts::v1::transport::Packet>();
        // получить индекс очередной точки для посылки на сервер
        boost::asio::async_write(
            m_socket,
            boost::asio::buffer(pkg->header()),
            [self = shared_from_this(), pkg = std::move(pkg)](
                const boost::system::error_code &ec,
                std::size_t bytes_transferred) mutable
            {
                if (!ec)
                {
                    if (pkg->frame_data_length() > 0)
                    {
                        self->do_write_frame(std::move(pkg));
                    }
                    else
                    {
                        // увеличить количество попыток отправки точек
                        self->do_write_header();
                    }
                }
                else
                {
                    // увеличить количество попыток отправки точек
                    // вернуть ошибку в main
                    std::cerr << "Error writing header: " << ec.message() << std::endl;
                }
            });
    }

    void
    do_write_frame(std::unique_ptr<egts::v1::transport::Packet> pkg)
    {
        boost::asio::async_write(
            m_socket,
            boost::asio::buffer(pkg->frame()),
            [self = shared_from_this()](
                const boost::system::error_code &ec,
                std::size_t bytes_transferred)
            {
                if (!ec)
                {
                    // увеличить количество попыток отправки точек
                    self->do_write_header();
                }
                else
                {
                    // увеличить количество попыток отправки точек
                    // вернуть ошибку в main
                    std::cerr << "Error writing frame: " << ec.message() << std::endl;
                }
            });
    }

    void
    do_read_header()
    {
        std::array<unsigned char, egts::v1::transport::header_length> header;
        boost::asio::async_read(
            m_socket,
            boost::asio::buffer(header),
            [self = shared_from_this(), &header](
                const boost::system::error_code &ec,
                std::size_t bytes_transferred)
            {
                if (!ec)
                {
                    auto pkg = std::make_unique<egts::v1::transport::Packet>();
                    auto err = pkg->parse_header(header);
                    if (err == egts::v1::error::Code::EGTS_PC_OK)
                    {
                        if (pkg->frame_data_length() > 0)
                        {
                            self->do_read_frame(std::move(pkg));
                        }
                        else
                        {
                            // Проверить, что если тип пакета — это ответ,  и фрейм пустой, то это приводит к неопределенному поведению.
                            // Если тип пакета новый, то сформировать ответ и поместить его в очередь для отправки на сервер.
                            self->do_read_header();
                        }
                    }
                    else
                    {
                        // Сформировать ответ на ошибочный пакет и сразу его отправит.
                        // Выйти по причине того, что буфер может содержать недочитанный мусор.
                        // вернуть ошибку в main
                        std::cerr << "Error reading header: " << ec.message() << std::endl;
                    }
                }
                else
                {
                    // вернуть ошибку в main
                    std::cerr << "Error reading header: " << ec.message() << std::endl;
                }
            });
    }

    void
    do_read_frame(std::unique_ptr<egts::v1::transport::Packet> pkg)
    {
        std::vector<unsigned char> frame(
            pkg->frame_data_length() +
                egts::v1::transport::crc_data_length,
            0);
        boost::asio::async_read(
            m_socket,
            boost::asio::buffer(frame),
            [self = shared_from_this(), pkg = std::move(pkg), &frame](
                const boost::system::error_code &ec,
                std::size_t bytes_transferred) mutable
            {
                if (!ec)
                {
                    auto err = pkg->parse_frame(std::move(frame));
                    if (err == egts::v1::error::Code::EGTS_PC_OK)
                    {
                        // Если тип пакета новый, то сформировать ответ и поместить его в очередь для отправки на сервер.
                        // Если это подтверждение получения пакета, то найти в очереди отправленных этот пакет и пометить его как полученный.
                        self->do_read_header();
                    }
                    else
                    {
                        // Сформировать ответ на ошибочный пакет и сразу его отправит.
                        // Выйти по причине того, что буфер может содержать недочитанный мусор.
                        // вернуть ошибку в main
                        std::cerr << "Error reading frame: " << ec.message() << std::endl;
                    }
                }
                else
                {
                    // вернуть ошибку в main
                    std::cerr << "Error reading frame: " << ec.message() << std::endl;
                }
            });
    }

  public:
    StateLessReaderWriter(boost::asio::ip::tcp::socket socket)
        : m_socket(std::move(socket)) {}

    void
    start_client()
    {
        // подписаться на очередь для получения точек
        do_write_header();
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
        std::make_shared<StateLessReaderWriter>(std::move(socket))->start_client();
        io_context.run();
    }
    catch (const std::exception &e)
    {
        // Смотреть на ошибку и, если необходимо, перезапустить клиента.
        std::cerr << "error: " << e.what() << std::endl;
    };
    return 0;
}
