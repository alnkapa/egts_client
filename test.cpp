#include "gtest/gtest.h"
#include <iostream>
#include "lib/endian/endian.hpp"

TEST(reverse, EndianTest) {
    std::uint32_t d = 0xafb1;
    std::uint32_t d1 = 0xb1af;
    std::cout << std::hex << d <<"\n";
    endian::reverse(d);
    std::cout << std::hex << d <<"\n";
    EXPECT_EQ(d,d1);
};
