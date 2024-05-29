// #include <iostream>
// #include "lib/egts/transport/transport.hpp"
// #include "lib/randy/randy.hpp"
// #include "lib/endian/endian.hpp"

// int main(int argv, const char **args)
// {
//     egts::transport::Packet tr{};
//     std::cin >> tr;
//     constexpr size_t max = 11;
//     auto bb = randy::random<max>();
//     char b[]{1, 2, 3, 4, 5, 6};
//     std::cin >> tr;
//     std::cout << tr << "\n";
//     return EXIT_SUCCESS;
// }

#include <cstdlib>
#include <iostream>

#include <boost/context/fiber.hpp>

namespace ctx = boost::context;

ctx::fiber bar( ctx::fiber && f) {
    do {
        std::cout << "bar\n";
        f = std::move( f).resume();
    } while ( f);
    return std::move( f);
}

int main() {
    ctx::fiber f{ bar };
    do {
        std::cout << "foo\n";
        f = std::move( f).resume();
    } while ( f);
    std::cout << "main: done" << std::endl;
    return EXIT_SUCCESS;
}