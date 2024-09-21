#include "boost/asio/write.hpp"
#include "my_globals.h"
#include <cstdlib>
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
            auto mes = std::move(g_send_queue.pop()); // lock
            if (std::holds_alternative<egts::v1::transport::Packet>(mes))
            {
                const auto &pkg = std::get<egts::v1::transport::Packet>(mes);
                boost::asio::write(
                    socket,
                    boost::asio::buffer(pkg.buffer()),
                    boost::asio::transfer_all());
            }
            else if (std::holds_alternative<egts::v1::record::subrecord::SrResultCode>(mes)) // auth result
            {
                const auto &rez = std::get<egts::v1::record::subrecord::SrResultCode>(mes);
                auto err = rez.error();
                if (!err.OK())
                {
                    std::cerr << "transport: auth: error: " << err.what() << std::endl;
                    break;
                }
                else
                {
                    // запустить чтение файла NMIE
                    std::cout << "transport: auth: ok" << std::endl;
                }
            }
            else if (std::holds_alternative<egts::v1::record::subrecord::SRRecordResponse>(mes)) // status of sent records
            {
                const auto &rez = std::get<egts::v1::record::subrecord::SRRecordResponse>(mes);
                // подтверждение отправленной записи, можно слать еще
            }
            else if (std::holds_alternative<Done>(mes)) // reader has finished execution.
            {
                break;
            }
            else
            {
                // ?
            }
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
    if (socket.is_open())
    {
        socket.close(); // reader will finish execution.
    }
    return EXIT_SUCCESS;
}
