#include "my_globals.h"

int
main(int argc, char *argv[])
{
    using boost::asio::ip::tcp;
    using namespace egts::v1;
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
    transport::Packet pkg{};
    pkg.buffer();
    // run reader
    std::thread reader(my_read, std::ref(socket));

    // // auth step
    // try
    // {
    //     // record::subrecord::SrTermIdentity i{};
    //     // i.IMEI = {};
    //     // i.buffer_size = 512;

    //     // make_package(i.buffer());
    // }
    // catch (const std::exception &e)
    // {
    //     // Смотреть на ошибку и, если необходимо, перезапустить клиента.
    //     std::cerr << "error: " << e.what() << std::endl;
    //     return EXIT_FAILURE;
    // };

    // // run sender
    // while (true)
    // {
    //     auto pkg = std::move(g_send_queue.pop()); // lock
    //     try
    //     {
    //         boost::asio::write(socket, boost::asio::buffer(pkg.buffer()), boost::asio::transfer_all());
    //     }
    //     catch (const boost::system::error_code &ec)
    //     {
    //         std::cerr << "transport: write: error: " << ec.message() << std::endl;
    //         break;
    //     }
    // }
    reader.join();
    return EXIT_SUCCESS;
}
