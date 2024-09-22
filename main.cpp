#include "boost/asio/write.hpp"
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
        // "863921034878280" -- АИмитатор 109
        // TODO: ask from cmd line
        i.IMEI = {'8', '6', '3', '9', '2', '1', '0', '3', '4', '8', '7', '8', '2', '8', '0'};
        i.buffer_size = 512;

        auto sub = egts::v1::record::subrecord::wrapper(
            egts::v1::record::subrecord::Type::EGTS_SR_TERM_IDENTITY,
            i.buffer());

        auto record_number = g_record_number++;

        auto rec = egts::v1::record::wrapper(
            record_number,
            egts::v1::record::ServiceType::EGTS_AUTH_SERVICE,
            egts::v1::record::ServiceType::EGTS_AUTH_SERVICE,
            std::move(sub));

        egts::v1::transport::Packet new_pkg{};
        new_pkg.identifier(g_packet_identifier++);

        new_pkg.set_frame(std::move(rec));

        boost::asio::write(
            socket,
            boost::asio::buffer(new_pkg.buffer()),
            boost::asio::transfer_all());
    }
    catch (const boost::system::error_code &err) // Connection errors
    {
        std::cerr << "transport: read: error: " << err.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception &err) // Other errors
    {
        std::cerr << "transport: read: error: " << err.what() << std::endl;
        return EXIT_FAILURE;
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
                    std::cout << "transport: auth: ok" << std::endl;
                    // run file reader
                    // TODO: ask from cmd line
                    auto file_ptr = std::make_shared<std::ifstream>("nmea.txt");
                    if (!file_ptr->is_open())
                    {
                        std::cerr << "transport: error open file nmea.txt" << std::endl;
                        break;
                    }
                    std::thread reader(my_read_file, file_ptr); // to exit g_keep_running = false
                    reader.detach();
                }
            }
            else if (std::holds_alternative<egts::v1::record::subrecord::SRRecordResponse>(mes)) // status of sent records
            {
                const auto &rez = std::get<egts::v1::record::subrecord::SRRecordResponse>(mes);
                // TODO: для загрузки файлов, будет нужно потом
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
    g_keep_running = false; // read_file will finish execution.
    if (socket.is_open())
    {
        socket.close(); // reader will finish execution.
    }
    return EXIT_SUCCESS;
}
