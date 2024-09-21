#include "my_globals.h"

int
main(int argc, char *argv[])
{
    using boost::asio::ip::tcp;
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
    // To exit, you need to close the socket.
    std::thread reader(my_read, std::ref(socket));
    reader.detach();

    // Authorization stage
    egts::v1::record::subrecord::SrTermIdentity i{};
    i.IMEI = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    i.buffer_size = 512;

    egts::v1::record::subrecord::wrapper(
        egts::v1::record::subrecord::Type::EGTS_SR_TERM_IDENTITY,
        i.buffer());

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
    return EXIT_SUCCESS;
}
