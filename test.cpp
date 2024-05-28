#include "gtest/gtest.h"
#include <iostream>
#include "lib/endian/endian.hpp"
#include <netinet/in.h>

TEST(reverse, EndianTest)
{
    std::uint32_t d = 0xafb1;
    // convert to big-endian
    auto test_d = htonl(d);
    auto get_d = endian::covert_to<endian::Order::big>(d);
    EXPECT_EQ(test_d, get_d);
};
