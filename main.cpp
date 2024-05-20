#include <iostream>
#include "lib/egts/transport/transport.hpp"

int main(int argv, const char **args)
{

    egts::transport::Packet tr{};
    std::cin >> tr;
    std::cout << "prefix:" << std::hex << tr.flags.prefix << "\n";
    std::cout << "route:" << std::hex << tr.flags.route << "\n";
    std::cout << "encryption_algorithm:" << std::hex << tr.flags.encryption_algorithm << "\n";
    std::cout << "compressed:" << std::hex << tr.flags.compressed << "\n";
    egts::transport::getPriority(tr.flags.pr);
    std::cout << "Priority:" << egts::transport::getPriority(tr.flags.pr) << "\n";
    std::cout << "\n";

    return EXIT_SUCCESS;
}