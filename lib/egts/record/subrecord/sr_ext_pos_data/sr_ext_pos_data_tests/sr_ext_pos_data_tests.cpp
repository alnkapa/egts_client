#include "error.h"
#include <gtest/gtest.h>
#include <record/subrecord/sr_ext_pos_data/sr_ext_pos_data.h>

TEST(EGTS_SR_EXT_POS_DATA, BasicTests)
{
    egts::v1::record::subrecord::SrExtPosData rd{};
    rd.vertical_dilution_of_precision = 100;
    rd.horizontal_dilution_of_precision = 200;
    rd.position_dilution_of_precision = 300;
    rd.satellites = 5;
    rd.navigation_system = 0x0001;
    auto buf = rd.buffer();
    egts::v1::record::subrecord::SrExtPosData rd1{};
    try
    {
        rd1.parse(buf);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }
    EXPECT_EQ(rd.vertical_dilution_of_precision, rd1.vertical_dilution_of_precision);
    EXPECT_EQ(rd.horizontal_dilution_of_precision, rd1.horizontal_dilution_of_precision);
    EXPECT_EQ(rd.position_dilution_of_precision, rd1.position_dilution_of_precision);
    EXPECT_EQ(rd.satellites, rd1.satellites);
    EXPECT_EQ(rd.navigation_system, rd1.navigation_system);

    rd1.navigation_system = 0;
    rd1.horizontal_dilution_of_precision = 0;
    rd.navigation_system = 0;
    rd.horizontal_dilution_of_precision = 0;
    buf = rd1.buffer();

    try
    {
        rd.parse(buf);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }
    EXPECT_EQ(rd.vertical_dilution_of_precision, rd1.vertical_dilution_of_precision);
    EXPECT_EQ(rd.horizontal_dilution_of_precision, rd1.horizontal_dilution_of_precision);
    EXPECT_EQ(rd.position_dilution_of_precision, rd1.position_dilution_of_precision);
    EXPECT_EQ(rd.satellites, rd1.satellites);
    EXPECT_EQ(rd.navigation_system, rd1.navigation_system);
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
