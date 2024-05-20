#include <iostream>
#include <cstdlib>
#include "lib/egts/transport/transport.hpp"

int main(int argv, const char **args)
{
    egts::transport::Packet tr{};
    // int dd;
    // std::cin >> dd;
    // std::cin >> tr;
    // Добрый день. О каком промежутки времени идет речь?

    uint d = 0xafb1;
    uint d1 = 0xb1af;
    std::cout << std::hex << d << "\n";
    auto d2 =  endian::reverse(d);
    std::cout << std::hex << d2 << "\n";
    std::cout << std::hex << d << "\n";
    return EXIT_SUCCESS;
    for (unsigned char i = 0; i < 255; i++)
    {
        tr.raw_flags = i;
        std::cout << "i:" << std::hex << static_cast<int>(i) << "\n";
        std::cout << "prefix:" << std::hex << tr.flags.prefix << "\n";
        std::cout << "route:" << std::hex << tr.flags.route << "\n";
        std::cout << "encryption_algorithm:" << std::hex << tr.flags.encryption_algorithm << "\n";
        std::cout << "compressed:" << std::hex << tr.flags.compressed << "\n";
        tr.flags.pr;
        egts::transport::getPriority(tr.flags.pr);
        std::cout << "Priority:" << egts::transport::getPriority(tr.flags.pr) << "\n";
        std::cout << "\n";
    };

    return EXIT_SUCCESS;
}