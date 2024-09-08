// #include "gtest/gtest.h"
// #include <iostream>
// #include <iostream>


// TEST(header_test_1, EgtsTransportPacket)
// {
//     ::testing::internal::CaptureStdout();
//     egts::v1::transport::Packet tr{};    
//     // первые 3 байт
//     // uint8_t b[]{{1},{2},{3},{4}};
//     // tr.parseStep1(b);    
//     // std::array<std::uint8_t, 9> data = {};
//     std::uint32_t d1 = 0xafb1;
//     // convert to big-endian
//     // auto test_d = htonl(d);
//     // auto get_d = endian::reverse(d);
//     // std::cout << tr << std::endl;
//     std::cout << "This is a standard output message." << std::endl;
//     std::string captured_stdout = testing::internal::GetCapturedStdout();
//     EXPECT_EQ("This is a standard output message.\n", captured_stdout);
//     // EXPECT_EQ(d, d1);
// };
