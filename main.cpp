#include <iostream>
// #include <boost/asio.hpp>
// #include "lib/egts/transport/all.hpp"

#include "lib/egts/egts.h"

int main(int argc, char *argv[]) {
    egts::v1::Buffer buffer1{
        uint8_t(1),
        uint8_t(2),
        uint16_t(256),
        uint32_t(2147483648),
        uint64_t(1<<6),
    };
    buffer1.printBuffer();

    // if (argc != 3) {
    //     std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
    //     return 1;
    // }
    // using namespace egts::v1::transport;
    // Packet p{};

    // p.pack();

    // boost::asio::io_context io_context;
    // using boost::asio::ip::tcp;
    // tcp::socket s(io_context);
    // try
    // {
    //     std::cout << "Connected to server" << std::endl;
    //     tcp::resolver resolver(io_context);

    //     boost::asio::connect(s, resolver.resolve(argv[1], argv[2]));

    //     std::string message = "Hello, server!";
    //     boost::asio::write(s, boost::asio::buffer(message));

    //     size_t reply_length = boost::asio::read(s,
    //     boost::asio::buffer(message));

    //     std::cout << "Reply is: ";
    //     std::cout << message;
    //     std::cout << "\n";
    // }
    // catch (std::exception &e)
    // {
    //     std::cerr << "Exception: " << e.what() << std::endl;
    // }

    return 0;
}

// #include <iostream>
// #include <cstddef> // size_t
// #include "lib/randy/randy.hpp"
// #include "lib/endian/endian.hpp"
// #include "lib/egts/transport/transport.hpp"

// int main(int argv, const char **args)
// {
//     auto bb = randy::Array<std::uint8_t, 2>{};
//     for (auto &v : bb)
//     {
//         std::cout << std::hex << static_cast<int>(v);
//     }
//     std::cout << "\n";

//     // egts::transport::Packet tr{};
//     // std::cin >> tr;

//     // std::cout << tr << "\n";

//     // вариант для чисел с плавающей точкой

//     return EXIT_SUCCESS;
// }
// std::string getPacketType(Type p)
//     // {
//     //     switch (p)
//     //     {
//     //     case Type::EGTS_PT_RESPONSE:
//     //         return "EGTS_PT_RESPONSE";
//     //     case Type::EGTS_PT_APPDATA:
//     //         return "EGTS_PT_APPDATA";
//     //     case Type::EGTS_PT_SIGNED_APPDATA:
//     //         return "EGTS_PT_SIGNED_APPDATA";
//     //     }
//     //     return "???";
//     // };

// #include <cstdlib>
// #include <iostream>

// #include <boost/context/fiber.hpp>

// namespace ctx = boost::context;

// ctx::fiber bar( ctx::fiber && f) {
//     do {
//         std::cout << "bar\n";
//         f = std::move( f).resume();
//     } while ( f);
//     return std::move( f);
// }

// int main() {
//     ctx::fiber f{ bar };
//     do {
//         std::cout << "foo\n";
//         f = std::move( f).resume();
//     } while ( f);
//     std::cout << "main: done" << std::endl;
//     return EXIT_SUCCESS;
// }