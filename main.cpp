#include "globals.h"
#include "my_globals.h"
#include "record/subrecord/firmware/firmware.h"
#include "record/subrecord/sr_command_data/sr_command_data.h"
#include "record/subrecord/sr_module_data/sr_module_data.h"
#include <cstdlib>
#include <iostream>
#include <regex>
#include <string>
#include <unistd.h> // getopt
#include <utility>
#include <variant>

void
printHelp()
{
    std::cout << "Usage: program [options]\n"
              << "Options:\n"
              << "  -h, --help                  Display help on usage and available parameters.\n"
              << "  -a [<host>]:<port>          Specify the server address to which the client will connect. For example: -a :5000, -a localhost:5000\n"
              << "  -e <IMEI>                   Specify the IMEI that will be used for authentication. For example: -e 863921034878280\n"
              << "  -n <file_path>              Specify the path to the file containing NMEA data. For example: -n nmea.txt\n";
}

bool
isValidIMEI(const char *imei)
{
    std::regex imeiRegex("^[0-9a-f]{14}$|^\\d{15}$|^\\d{18}$");
    return std::regex_match(imei, imeiRegex);
}

int
main(int argc, char *argv[])
{
    egts::v1::record::subrecord::SrTermIdentity i{};
    i.buffer_size = 512;
    std::string port = "36329";
    std::string host = "test.shatl-t.ru";

    int opt;
    while ((opt = getopt(argc, argv, "ha:e:n:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            printHelp();
            return 0;
        case 'a':
        {
            std::string address{optarg};
            size_t colon_pos = address.find(':');
            if (colon_pos != std::string::npos)
            {
                host = address.substr(0, colon_pos);
                port = address.substr(colon_pos + 1);
            }
            else
            {
                std::cerr << "Invalid address: " << optarg << std::endl;
                return 1;
            }
            break;
        }
        case 'e':
            if (!isValidIMEI(optarg))
            {
                std::cerr << "Invalid IMEI: " << optarg << ". IMEI must match the specified format." << std::endl;
                return 1;
            }
            std::copy(optarg, optarg + strlen(optarg), i.IMEI.begin());
            break;
        case 'n':
            g_nmea_file = std::ifstream(optarg);
            if (!g_nmea_file.is_open())
            {
                std::cerr << "error open file: " << optarg << std::endl;
                return 1;
            }
            break;
        case '?':
            if (optopt == 'a' || optopt == 'e' || optopt == 'n')
            {
                std::cerr << "Option -" << static_cast<char>(optopt) << " requires an argument.\n";
            }
            else
            {
                std::cerr << "Unknown option: -" << static_cast<char>(optopt) << "\n";
            }
            return 1;
        default:
            return 1;
        }
    }

    using boost::asio::ip::tcp;
    boost::asio::io_context io_context{};
    tcp::socket socket(io_context);
    try
    {
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve(host, port));
        std::cout << "connecting address: " << host << ":" << port << "\n";
    }
    catch (const boost::system::error_code &err) // Connection errors
    {
        std::cerr << "error: " << err.message() << "\n";
        return EXIT_FAILURE;
    }
    catch (const std::exception &err) // Other errors
    {
        std::cerr << "error: " << err.what() << "\n";

        return EXIT_FAILURE;
    }

    // run reader
    // To exit, you need to close the socket.
    std::thread reader(my_read, std::ref(socket));
    reader.detach();

    // send auth and module
    try
    {
        auto sub = egts::v1::record::subrecord::wrapper(
            egts::v1::record::subrecord::Type::EGTS_SR_TERM_IDENTITY,
            i.buffer());

        // TODO: ask from cmd line
        egts::v1::record::subrecord::SrModuleData m{
            1,                                                                         // module_type
            2,                                                                         // vendor_identifier
            0x1010,                                                                    // firmware_version
            0x2020,                                                                    // software_version
            1,                                                                         // modification
            1,                                                                         // state
            "client_egts_serial_number",                                               // serial_number
            {"ICCID:897010269724308273", "VIN:EAA236021J1001200", "client_egts_desc2"} // description
        };
        sub += egts::v1::record::subrecord::wrapper(
            egts::v1::record::subrecord::Type::EGTS_SR_MODULE_DATA,
            m.buffer());

        auto buffer = make_new_packet(
            egts::v1::record::ServiceType::EGTS_AUTH_SERVICE,
            std::move(sub));
        if (boost::asio::write(
                socket,
                boost::asio::buffer(buffer),
                boost::asio::transfer_all()) != buffer.size())
        {
            throw std::runtime_error("send size error");
        }
    }
    catch (const boost::system::error_code &err) // Connection errors
    {
        std::cerr << "error: " << err.message() << "\n";
        return EXIT_FAILURE;
    }
    catch (const std::exception &err) // Other errors
    {
        std::cerr << "error: " << err.what() << "\n";
        return EXIT_FAILURE;
    }

    // Main
    while (true)
    {
        try
        {
            std::visit(
                [&socket](auto &&arg)
                {
                    using T = std::decay_t<decltype(arg)>;
                    using namespace egts::v1::record::subrecord;
                    using namespace egts::v1::transport;
                    if constexpr (std::is_same_v<T, Packet>)
                    {
                        arg.identifier(g_packet_identifier++);
                        if (boost::asio::write(
                                socket,
                                boost::asio::buffer(arg.buffer()),
                                boost::asio::transfer_all()) != arg.buffer().size())
                        {
                            throw std::runtime_error("send size error");
                        }
                    }
                    else if constexpr (std::is_same_v<T, SRRecordResponse>)
                    {
                        // Обработка SRRecordResponse
                        std::cout << "Processing SRRecordResponse" << std::endl;
                    }
                    else if constexpr (std::is_same_v<T, SrResultCode>)
                    {
                        // Обработка SrResultCode
                        std::cout << "Processing SrResultCode" << std::endl;
                    }
                    else if constexpr (std::is_same_v<T, SrCommandData>)
                    {
                        // Обработка SrCommandData
                        std::cout << "Processing SrCommandData" << std::endl;
                    }
                    else if constexpr (std::is_same_v<T, SrPartData>)
                    {
                        // Обработка SrPartData
                        std::cout << "Processing SrPartData" << std::endl;
                    }
                    else if constexpr (std::is_same_v<T, SrFullData>)
                    {
                        // Обработка SrFullData
                        std::cout << "Processing SrFullData" << std::endl;
                    }
                    else if constexpr (std::is_same_v<T, Done>)
                    {
                        // Обработка Done
                        std::cout << "Processing Done" << std::endl;
                    }
                },
                g_send_queue.pop());
        }
        catch (const egts::v1::error::Error &err) // Protocol errors
        {
            std::cerr << "error: " << err.what() << "\n";
            break;
        }
        catch (const boost::system::error_code &err) // Connection errors
        {
            std::cerr << "error: " << err.message() << "\n";
            break;
        }
        catch (const std::exception &err) // Other errors
        {
            std::cerr << "error: " << err.what() << "\n";
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
