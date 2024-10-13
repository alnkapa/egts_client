#include "error.h"
#include <gtest/gtest.h>
#include <record/subrecord/firmware/firmware.h>
#include <utility>

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

TEST(FIRMWARE_SR_PART_DATA, BasicTests)
{
    egts::v1::record::subrecord::SrPartData partData{};
    egts::v1::record::subrecord::SrPartData partData1{};
    partData.identity = 0x1234;
    partData.part_number = 1;
    partData.expected_parts_quantity = 3;
    egts::v1::record::subrecord::ObjectDataHeader odh{};
    odh.file_name("test_file.bin");
    odh.version = 0x0222;
    odh.crc_value = 12345;
    odh.component_or_module_identifier = 5;
    odh.object_type = egts::v1::record::subrecord::ObjectType::CONFIG;
    odh.module_type = egts::v1::record::subrecord::ModuleType::PERIPHERALS;
    partData.odh(std::move(odh));
    partData.data({0x01, 0x02, 0x03});
    auto buf = partData.buffer();
    egts::v1::record::subrecord::payload_type s(buf.cbegin(), buf.cend());
    auto ptr = s.begin();
    try
    {
        partData1.parse(s);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }

    EXPECT_EQ(partData1.identity, 0x1234);
    EXPECT_EQ(partData1.part_number, 1);
    EXPECT_EQ(partData1.expected_parts_quantity, 3);
    auto odh_copy = partData1.odh();
    EXPECT_EQ(odh_copy.file_name(), "test_file.bin");
    EXPECT_EQ(odh_copy.version, 0x0222);
    EXPECT_EQ(odh_copy.crc_value, 12345);
    EXPECT_EQ(odh_copy.component_or_module_identifier, 5);
    EXPECT_EQ(odh_copy.object_type, egts::v1::record::subrecord::ObjectType::CONFIG);
    EXPECT_EQ(odh_copy.module_type, egts::v1::record::subrecord::ModuleType::PERIPHERALS);
    auto d = partData1.data();
    EXPECT_EQ(d[0], 0x01);
    EXPECT_EQ(d[1], 0x02);
    EXPECT_EQ(d[2], 0x03);
}

TEST(FIRMWARE_SR_FULL, BasicTests)
{
    egts::v1::record::subrecord::SrFullData partData{};
    egts::v1::record::subrecord::SrFullData partData1{};
    egts::v1::record::subrecord::ObjectDataHeader odh{};
    odh.file_name("test_file.bin");
    odh.version = 0x0222;
    odh.crc_value = 12345;
    odh.component_or_module_identifier = 5;
    odh.object_type = egts::v1::record::subrecord::ObjectType::CONFIG;
    odh.module_type = egts::v1::record::subrecord::ModuleType::PERIPHERALS;
    partData.odh(std::move(odh));
    partData.data({0x01, 0x02, 0x03});
    auto buf = partData.buffer();
    egts::v1::record::subrecord::payload_type s(buf.cbegin(), buf.cend());
    auto ptr = s.begin();
    try
    {
        partData1.parse(s);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }

    auto odh_copy = partData1.odh();
    EXPECT_EQ(odh_copy.file_name(), "test_file.bin");
    EXPECT_EQ(odh_copy.version, 0x0222);
    EXPECT_EQ(odh_copy.crc_value, 12345);
    EXPECT_EQ(odh_copy.component_or_module_identifier, 5);
    EXPECT_EQ(odh_copy.object_type, egts::v1::record::subrecord::ObjectType::CONFIG);
    EXPECT_EQ(odh_copy.module_type, egts::v1::record::subrecord::ModuleType::PERIPHERALS);
    auto d = partData1.data();
    EXPECT_EQ(d[0], 0x01);
    EXPECT_EQ(d[1], 0x02);
    EXPECT_EQ(d[2], 0x03);
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
