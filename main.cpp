#include "boost/asio/write.hpp"
#include "my_globals.h"
#include <cstdlib>
#include <ios>
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
        boost::asio::connect(socket, resolver.resolve("alfa.shatl-t.ru", "36329"));
        // boost::asio::connect(socket, resolver.resolve("localhost", "12345"));
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
        // send sr_term_identity
        egts::v1::record::subrecord::SrTermIdentity i{};
        i.IMEI = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        i.buffer_size = 512;

        egts::v1::transport::Packet new_pkg{};
        new_pkg.identifier(g_packet_identifier++);

        auto record_number = g_record_number++;

        auto sub = egts::v1::record::subrecord::wrapper(
            egts::v1::record::subrecord::Type::EGTS_SR_TERM_IDENTITY,
            i.buffer());

        auto rec = egts::v1::record::wrapper(
            record_number,
            egts::v1::record::ServiceType::EGTS_AUTH_SERVICE,
            egts::v1::record::ServiceType::EGTS_AUTH_SERVICE,
            std::move(sub));

        new_pkg.set_frame(std::move(rec));

        std::cout << new_pkg.buffer().size() << std::endl;
        for (auto v : new_pkg.buffer())
        {
            std::cout << std::hex << static_cast<int>(v);
        }
        std::cout << std::endl;

        boost::asio::write(
            socket,
            boost::asio::buffer(new_pkg.buffer()),
            boost::asio::transfer_all());
    }
    catch (const boost::system::error_code &err) // Connection errors
    {
        std::cerr << "transport: read: error: " << err.what() << std::endl;
    }
    catch (const std::exception &err) // Other errors
    {
        std::cerr << "transport: read: error: " << err.what() << std::endl;
    }

    // Main
    while (true)
    {
        try
        {
            auto mes = std::move(g_send_queue.pop()); // lock
            if (std::holds_alternative<egts::v1::transport::Packet>(mes))
            {
                auto &pkg = std::get<egts::v1::transport::Packet>(mes);
                pkg.identifier(g_packet_identifier++);
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
        catch (const egts::v1::error::Error &err) // Protocol errors
        {
            std::cerr << "transport: read: error: " << err.what() << std::endl;
            break;
        }
        catch (const boost::system::error_code &err) // Connection errors
        {
            std::cerr << "transport: read: error: " << err.what() << std::endl;
            break;
        }
        catch (const std::exception &err) // Other errors
        {
            std::cerr << "transport: read: error: " << err.what() << std::endl;
            break;
        }
    }

    if (socket.is_open())
    {
        socket.close(); // reader will finish execution.
    }
    return EXIT_SUCCESS;
}
