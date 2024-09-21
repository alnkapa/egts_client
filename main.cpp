#include "boost/asio/write.hpp"
#include "my_globals.h"
#include <memory>

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
    catch (const boost::system::error_code &err) // Connection errors
    {
        std::cerr << "transport: read: error: " << err.what() << std::endl;
    }
    catch (const std::exception &err) // Other errors
    {
        std::cerr << "transport: read: error: " << err.what() << std::endl;
    }

    // run reader
    // To exit, you need to close the socket.
    std::thread reader(my_read, std::ref(socket));
    reader.detach();

    // subscribe for auth result
    auto sr_result_code_callback = std::make_shared<G_PUB_RESULT_CODE_TYPE::Function>(
        [](const egts::v1::record::subrecord::SrResultCode &rez) {

        });
    g_pub_result_code.subscribe(sr_result_code_callback);

    // Subscribe to the status of sent records
    auto sr_record_response_callback = std::make_shared<G_PUB_RECORD_TYPE::Function>(
        [](const egts::v1::record::subrecord::SRRecordResponse &rez) {

        });
    g_pub_record.subscribe(sr_record_response_callback);
    try
    {
        {
            // send sr_term_identity
            egts::v1::record::subrecord::SrTermIdentity i{};
            i.IMEI = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
            i.buffer_size = 512;

            auto pkg = make_package(i);

            boost::asio::write(
                socket,
                boost::asio::buffer(pkg.buffer()),
                boost::asio::transfer_all());
        }

        // Main
        while (true)
        {
            auto pkg = std::move(g_send_queue.pop()); // lock

            boost::asio::write(
                socket,
                boost::asio::buffer(pkg.buffer()),
                boost::asio::transfer_all());

            // wait for sr_result_code
        }
    }
    catch (const egts::v1::error::Error &err) // Protocol errors
    {
        std::cerr << "transport: read: error: " << err.what() << std::endl;
    }
    catch (const boost::system::error_code &err) // Connection errors
    {
        std::cerr << "transport: read: error: " << err.what() << std::endl;
    }
    catch (const std::exception &err) // Other errors
    {
        std::cerr << "transport: read: error: " << err.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
