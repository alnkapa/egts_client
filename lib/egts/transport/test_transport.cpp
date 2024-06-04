#include "gtest/gtest.h"
#include <iostream>
#include <iostream>
#include "all.hpp"

TEST(header_test_1, EgtsTransportPacket)
{
    ::testing::internal::CaptureStdout();
    egts::v1::transport::Packet tr{};    
    uint8_t b[3] = {{1},{2},{3}};
    tr.parseStep1(b);
    // первые 9 байт
    // std::array<std::uint8_t, 9> data = {};
    std::uint32_t d1 = 0xafb1;
    // convert to big-endian
    // auto test_d = htonl(d);
    // auto get_d = endian::reverse(d);
    // std::cout << tr << std::endl;
    std::cout << "This is a standard output message." << std::endl;
    std::string captured_stdout = testing::internal::GetCapturedStdout();
    EXPECT_EQ("This is a standard output message.\n", captured_stdout);
    // EXPECT_EQ(d, d1);
};
