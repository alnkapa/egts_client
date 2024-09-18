#include "lib/egts/error/error.h"
#include "lib/egts/record/record.h"
#include "lib/egts/subrecord/sr_record_response/sr_record_response.h"
#include "lib/egts/subrecord/sr_result_code/sr_result_code.h"
#include "lib/egts/subrecord/sr_term_identity/sr_term_identity.h"
#include "lib/egts/subrecord/subrecord.h"
#include "lib/egts/transport/transport.h"
#include "globals.h"
#include <iostream>
#include <thread>

using namespace egts::v1;

std::atomic<bool> g_running(true);
MyPacketQueue g_send_queue{};


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
