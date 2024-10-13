#include "error.h"
#include <gtest/gtest.h>
#include <record/subrecord/sr_module_data/sr_module_data.h>

TEST(SR_MODULE_DATA_PARSE, BasicTests)
{
    egts::v1::record::subrecord::SrModuleData rd{};
    egts::v1::record::subrecord::SrModuleData rd1{};
    rd.module_type = 1;
    rd.vendor_identifier = 2;
    rd.firmware_version = 3;
    rd.software_version = 4;
    rd.modification = 5;
    rd.state = 6;
    rd.serial_number = {1, 2, 3, 4, 5, 6};
    rd.description.emplace_back("Desc");
    rd.description.emplace_back("Desc1");
    rd.description.emplace_back("Desc2");
    auto buffer = rd.buffer();
    try
    {
        rd1.parse(buffer);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }
    EXPECT_EQ(rd.module_type, rd1.module_type);
    EXPECT_EQ(rd.vendor_identifier, rd1.vendor_identifier);
    EXPECT_EQ(rd.firmware_version, rd1.firmware_version);
    EXPECT_EQ(rd.software_version, rd1.software_version);
    EXPECT_EQ(rd.modification, rd1.modification);
    EXPECT_EQ(rd.state, rd1.state);
    EXPECT_EQ(rd.serial_number, rd1.serial_number);
    EXPECT_EQ(rd.description.size(), rd1.description.size());

    for (size_t i = 0; i < rd.description.size(); ++i)
    {
        EXPECT_EQ(rd.description[i], rd1.description[i]);
    }
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
