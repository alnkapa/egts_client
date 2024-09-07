#include <gtest/gtest.h>
#include <cstdint>
#include <vector>
#include "../crc.h"

namespace egts::v1 {

TEST(CRC8Test, BasicTests) {
    std::vector<std::uint8_t> data1 = {'1', '2', '3', '4', '5',
                                       '6', '7', '8', '9'};
    EXPECT_EQ(crc8(data1.begin(), data1.end()), 0xF7);
}

TEST(CRC16Test, BasicTests) {
    std::vector<std::uint8_t> data1 = {'1', '2', '3', '4', '5',
                                       '6', '7', '8', '9'};
    EXPECT_EQ(crc16(data1.begin(), data1.end()), 0x29B1);
}

}  // namespace egts::v1

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
