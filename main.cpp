#include "lib/egts/error/error.h"
#include "lib/egts/record/record.h"
#include "lib/egts/transport/transport.h"
#include "queue.h"
#include <atomic>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using boost::asio::ip::tcp;
using namespace egts::v1;

std::atomic<bool> g_running(true);
MyPacketQueue g_send_queue{};

void
my_read(tcp::socket &socket) noexcept
{
    transport::header_buffer_type header_buffer{};
    boost::system::error_code ec;
    error::Error err;
    while (g_running)
    {
        // read header
        boost::asio::read(socket, boost::asio::buffer(header_buffer), boost::asio::transfer_all(), ec);
        if (ec)
        {
            // TODO: обработать ошибку
            g_running = false;
            std::cerr << "transport: read: error: " << ec.message() << std::endl;
            return;
        }
        transport::Packet pkg{};
        err = pkg.parse_header(header_buffer);
        if (err != error::Code::EGTS_PC_OK)
        {
            // TODO: обработать ошибку
            std::cerr << "transport: read: error: " << err.what() << std::endl;
            return;
        }
        if (pkg.frame_data_length() == 0)
        {
            continue;
        }

        // read payload
        transport::frame_buffer_type frame_buffer(pkg.frame_data_length() + transport::crc_data_length, 0);
        boost::asio::read(socket, boost::asio::buffer(frame_buffer), boost::asio::transfer_all(), ec);
        if (ec)
        {
            // TODO: обработать ошибку
            g_running = false;
            std::cerr << "transport: read: error: " << ec.message() << std::endl;
            return;
        }
        err = pkg.parse_frame(std::move(frame_buffer));
        if (err != error::Code::EGTS_PC_OK)
        {
            // TODO: обработать ошибку
            std::cerr << "transport: read: error: " << err.what() << std::endl;
            return;
        }

        std::cout << "transport: read: size: " << transport::header_length + pkg.frame_data_length() + transport::crc_data_length << std::endl;
        // record level
        auto buffer = pkg.get_frame();
        auto ptr = buffer.begin();
        while (ptr != buffer.end())
        {
            record::Record rec{};
            auto err = rec.parse(buffer, ptr);
            if (err != error::Code::EGTS_PC_OK)
            {
                std::cerr << "transport: record: error: " << err.what() << std::endl;
                return;
            }
            
        }
    }
}

int
main(int argc, char *argv[])
{
    boost::asio::io_context io_context{};
    tcp::socket socket(io_context);
    try
    {
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve("alfa.shatl-t.ru", "34329"));
    }
    catch (const std::exception &e)
    {
        // Смотреть на ошибку и, если необходимо, перезапустить клиента.
        std::cerr << "error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    };
    // run reader
    std::thread receiver(my_read, std::ref(socket));
    receiver.detach();

    // run sender
    boost::system::error_code ec;
    while (g_running)
    {
        auto pkg = std::move(g_send_queue.pop()); // lock
        boost::asio::write(socket, boost::asio::buffer(pkg.buffer()), boost::asio::transfer_all(), ec);
        if (ec)
        {
            // TODO: обработать ошибку
            g_running = false;
            std::cerr << "transport: write: error: " << ec.message() << std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
