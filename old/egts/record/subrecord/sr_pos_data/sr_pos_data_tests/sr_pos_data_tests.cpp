#include "error.h"
#include <gtest/gtest.h>
#include <record/subrecord/sr_pos_data/sr_pos_data.h>

TEST(SR_POS_DATA_PARSE, BasicTests)
{

    egts::v1::record::subrecord::SrPosData rd1{};
    rd1.navigation_time = 1;
    rd1.latitude = 2;
    rd1.speed = 4;
    rd1.direction = 5;
    rd1.odometer = 6;
    rd1.digital_inputs = 7;
    rd1.source = 8;
    rd1.altitude = 9;
    rd1.source_data = 10;
    rd1.flags = 0xff;
    auto buf = rd1.buffer();
    egts::v1::record::subrecord::SrPosData rd{};
    try
    {
        rd.parse(buf);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }
    if (rd1.navigation_time != rd.navigation_time)
    {
        ADD_FAILURE() << "navigation_time";
    }
    if (rd1.latitude != rd.latitude)
    {
        ADD_FAILURE() << "latitude";
    }
    if (rd1.speed != rd.speed)
    {
        ADD_FAILURE() << "speed";
    }
    if (rd1.direction != rd.direction)
    {
        ADD_FAILURE() << "direction";
    }
    if (rd1.odometer != rd.odometer)
    {
        ADD_FAILURE() << "odometer";
    }
    if (rd1.digital_inputs != rd.digital_inputs)
    {
        ADD_FAILURE() << "digital_inputs";
    }
    if (rd1.source != rd.source)
    {
        ADD_FAILURE() << "source";
    }
    if (rd1.altitude != rd.altitude)
    {
        ADD_FAILURE() << "altitude";
    }
    if (rd1.source_data != rd.source_data)
    {
        ADD_FAILURE() << "source_data" << rd1.source_data << " : " << rd.source_data;
    }
    if (rd1.flags != rd.flags)
    {
        ADD_FAILURE() << "flags";
    }
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
