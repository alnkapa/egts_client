#include "gtest/gtest.h"
#include <iostream>
#include <netinet/in.h>

TEST(reverse, EndianTest)
{
    std::uint32_t d = 0xafb1;
    std::uint32_t d1 = 0xafb1;
    // convert to big-endian
    //auto test_d = htonl(d);
    //auto get_d = endian::reverse(d);
    EXPECT_EQ(d, d1);
};
