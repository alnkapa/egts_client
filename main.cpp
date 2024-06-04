#include <iostream>
#include <cstddef> // size_t
#include "lib/randy/randy.hpp"
#include "lib/endian/endian.hpp"
#include "lib/egts/transport/transport.hpp"



int main(int argv, const char **args)
{
    auto bb = randy::Array<std::uint8_t, 2>{};
    for (auto &v : bb)
    {
        std::cout << std::hex << static_cast<int>(v);
    }
    std::cout << "\n";

    // egts::transport::Packet tr{};
    // std::cin >> tr;

    // std::cout << tr << "\n";

    // вариант для чисел с плавающей точкой

    return EXIT_SUCCESS;
}
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