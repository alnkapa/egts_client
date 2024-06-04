#include <iostream>
#include "lib/egts/transport/transport.hpp"
#include "lib/randy/randy.hpp"
#include "lib/endian/endian.hpp"

int main(int argv, const char **args)
{
    auto bb = randy::Array<std::uint8_t, 2>{};
    for (auto &v : bb)
    {
        std::cout << std::hex << static_cast<int>(v);
    }
    std::cout << "\n";

    egts::transport::Packet tr{};

    // вариант для чисел с плавающей точкой

    return EXIT_SUCCESS;
}

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