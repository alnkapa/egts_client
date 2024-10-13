#include "error.h"
#include <gtest/gtest.h>
#include <record/subrecord/sr_command_data/sr_command_data.h>

TEST(SR_COMMAND_DATA_PARSE, BasicTests)
{
    egts::v1::record::subrecord::SrCommandData rd{};
    egts::v1::record::subrecord::SrCommandData rd1{};
    try
    {
        rd.command_identifier = 0x1234;
        rd.source_identifier = 0x5678;
        rd.charset = egts::v1::record::subrecord::CharSet::BINARY_DATA_2;
        rd.authorization_code({0xAB, 0xCD});
        rd.data.address = 0x0001;
        rd.data.code = 0x00EF;
        rd.command_type = egts::v1::record::subrecord::CommandType::CT_COM;
        rd.data.action = egts::v1::record::subrecord::DataAct::ADD_ACT;
        rd.data.size(1);
        rd.data.data({0x01});
        auto buf = rd.buffer();
        rd1.parse(buf);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }
    EXPECT_EQ(rd1.command_identifier, rd.command_identifier);
    EXPECT_EQ(rd1.source_identifier, rd.source_identifier);
    EXPECT_EQ(rd1.command_type, rd.command_type);
    EXPECT_EQ(rd1.charset, rd.charset);
    EXPECT_EQ(rd1.authorization_code().size(), rd.authorization_code().size());
    EXPECT_EQ(rd1.authorization_code()[0], rd.authorization_code()[0]);
    EXPECT_EQ(rd1.authorization_code()[1], rd.authorization_code()[1]);
    EXPECT_EQ(rd1.data.address, rd.data.address);
    EXPECT_EQ(rd1.data.code, rd.data.code);
    EXPECT_EQ(rd1.data.action, rd.data.action);
    EXPECT_EQ(rd1.data.size(), rd.data.size());
    EXPECT_EQ(rd1.data.data().size(), rd.data.data().size());
    EXPECT_EQ(rd1.data.data()[0], rd.data.data()[0]);

    try
    {
        rd.command_identifier = 0x1234;
        rd.source_identifier = 0x5678;
        rd.charset = egts::v1::record::subrecord::CharSet::BINARY_DATA_2;
        rd.authorization_code({0xAB, 0xCD});
        rd.data.address = 0x0001;
        rd.data.code = 0x00EF;
        rd.command_type = egts::v1::record::subrecord::CommandType::CT_COMCONF;
        rd.data.data({0x01});
        auto buf = rd.buffer();
        rd1.parse(buf);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }
    EXPECT_EQ(rd1.command_identifier, rd.command_identifier);
    EXPECT_EQ(rd1.source_identifier, rd.source_identifier);
    EXPECT_EQ(rd1.command_type, rd.command_type);
    EXPECT_EQ(rd1.charset, rd.charset);
    EXPECT_EQ(rd1.authorization_code().size(), rd.authorization_code().size());
    EXPECT_EQ(rd1.authorization_code()[0], rd.authorization_code()[0]);
    EXPECT_EQ(rd1.authorization_code()[1], rd.authorization_code()[1]);
    EXPECT_EQ(rd1.data.address, rd.data.address);
    EXPECT_EQ(rd1.data.code, rd.data.code);
    EXPECT_EQ(rd1.data.action, rd.data.action);
    EXPECT_EQ(rd1.data.size(), rd.data.size());
    EXPECT_EQ(rd1.data.data().size(), rd.data.data().size());
    EXPECT_EQ(rd1.data.data()[0], rd.data.data()[0]);
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
