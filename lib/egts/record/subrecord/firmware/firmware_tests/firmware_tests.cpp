#include "error.h"
#include <gtest/gtest.h>
#include <record/subrecord/firmware/firmware.h>

TEST(FIRMWARE_OBJECT_DATA_HEADER, BasicTests)
{
    egts::v1::record::subrecord::ObjectDataHeader rd{};
    rd.file_name("test_file.bin");
    rd.version = 0x0222;
    rd.crc_value = 12345;
    rd.component_or_module_identifier = 5;
    rd.object_type = egts::v1::record::subrecord::ObjectType::CONFIG;
    rd.module_type = egts::v1::record::subrecord::ModuleType::PERIPHERALS;
    auto buf = rd.buffer();
    egts::v1::record::subrecord::payload_type s(buf.cbegin(), buf.cend());
    auto ptr = s.begin();
    egts::v1::record::subrecord::ObjectDataHeader rd1{};
    try
    {
        rd1.parse(s, ptr);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }
    EXPECT_EQ(rd1.file_name(), "test_file.bin");
    EXPECT_EQ(rd1.version, 0x0222);
    EXPECT_EQ(rd1.crc_value, 12345);
    EXPECT_EQ(rd1.component_or_module_identifier, 5);
    EXPECT_EQ(rd1.object_type, egts::v1::record::subrecord::ObjectType::CONFIG);
    EXPECT_EQ(rd1.module_type, egts::v1::record::subrecord::ModuleType::PERIPHERALS);
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
